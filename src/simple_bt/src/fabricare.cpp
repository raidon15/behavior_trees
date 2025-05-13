#include <behaviortree_ros2/bt_action_node.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <control_msgs/action/follow_joint_trajectory.hpp>
#include <moveit_msgs/action/execute_trajectory.hpp>
#include <chrono>
#include <filesystem>
#include <fstream>
#include "behaviortree_cpp/xml_parsing.h"
#include <vector>
#include <sstream>
#include <algorithm>
#include "std_msgs/msg/bool.hpp"


std::vector<double> parseInputToVector(const std::string& input)
{
    std::vector<double> result;

    std::string clean_input = input;
    clean_input.erase(std::remove(clean_input.begin(), clean_input.end(), '['), clean_input.end());
    clean_input.erase(std::remove(clean_input.begin(), clean_input.end(), ']'), clean_input.end());

    std::stringstream ss(clean_input);
    std::string item;
    while (std::getline(ss, item, ','))
    {
        try
        {
            result.push_back(std::stod(item));
        }
        catch (const std::invalid_argument& e)
        {
            RCLCPP_ERROR(rclcpp::get_logger("BT"), "Invalid input: %s", item.c_str());
        }
    }

    return result;
}

class GenerateAndExecuteTrajectory : public BT::StatefulActionNode
{
public:
    GenerateAndExecuteTrajectory(const std::string& name, const BT::NodeConfig& config, const BT::RosNodeParams& params)
        : BT::StatefulActionNode(name, config), node_(params.nh)
    {
        // Create action client
        action_client_ = rclcpp_action::create_client<moveit_msgs::action::ExecuteTrajectory>(node_, "execute_trajectory");

        // Subscribe to the joint_states topic
        subscription_ = node_->create_subscription<sensor_msgs::msg::JointState>(
            "/joint_states", 10,
            [this](const sensor_msgs::msg::JointState::SharedPtr msg) {
                latest_joint_states_ = *msg;
                received_joint_states_ = true;
            });
    }

    static BT::PortsList providedPorts()
    {
        return {
            BT::InputPort<std::string>("final_positions")
        };
    }

    BT::NodeStatus onStart() override
    {
        std::string final_positions_str;

        if (!getInput("final_positions", final_positions_str))
        {
            RCLCPP_ERROR(node_->get_logger(), "Missing input [final_positions]");
            return BT::NodeStatus::FAILURE;
        }

        std::vector<double> final_positions = parseInputToVector(final_positions_str);

        if (final_positions.empty())
        {
            RCLCPP_ERROR(node_->get_logger(), "Input [final_positions] is empty");
            return BT::NodeStatus::FAILURE;
        }

        if (!received_joint_states_)
        {
            RCLCPP_ERROR(node_->get_logger(), "No joint states received yet");
            return BT::NodeStatus::FAILURE;
        }

        std::vector<double> initial_positions = latest_joint_states_.position;

        

        moveit_msgs::msg::RobotTrajectory trajectory;
        trajectory.joint_trajectory.joint_names = latest_joint_states_.name;


        if (initial_positions.size() != trajectory.joint_trajectory.joint_names.size() ||
            final_positions.size() != trajectory.joint_trajectory.joint_names.size())
        {
            RCLCPP_ERROR(node_->get_logger(), "Size mismatch between joint names and positions");
            return BT::NodeStatus::FAILURE;
        }


        trajectory_msgs::msg::JointTrajectoryPoint initial_point;
        initial_point.time_from_start = rclcpp::Duration(15, 0);
        initial_point.positions = initial_positions;
        trajectory.joint_trajectory.points.push_back(initial_point);

        trajectory_msgs::msg::JointTrajectoryPoint final_point;
        final_point.time_from_start = rclcpp::Duration(15, 0);
        final_point.positions = final_positions;
        trajectory.joint_trajectory.points.push_back(final_point);

        auto goal_msg = moveit_msgs::action::ExecuteTrajectory::Goal();
        goal_msg.trajectory = trajectory;

        auto send_goal_options = typename rclcpp_action::Client<moveit_msgs::action::ExecuteTrajectory>::SendGoalOptions();
        send_goal_options.goal_response_callback = [this](auto) { RCLCPP_INFO(node_->get_logger(), "Goal accepted"); };
        send_goal_options.result_callback = [this](const auto& result) {
            result_ = result.result->error_code.val == moveit_msgs::msg::MoveItErrorCodes::SUCCESS;
            received_result_ = true;
        };

        future_handle_ = action_client_->async_send_goal(goal_msg, send_goal_options);
        return BT::NodeStatus::RUNNING;
    }

    BT::NodeStatus onRunning() override
    {
        if (received_result_)
        {
            setOutput("success", result_);
            return result_ ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
        }
        return BT::NodeStatus::RUNNING;
    }

    void onHalted() override {}

private:
    rclcpp::Node::SharedPtr node_;
    rclcpp_action::Client<moveit_msgs::action::ExecuteTrajectory>::SharedPtr action_client_;
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr subscription_;
    sensor_msgs::msg::JointState latest_joint_states_;
    std::shared_future<typename rclcpp_action::ClientGoalHandle<moveit_msgs::action::ExecuteTrajectory>::SharedPtr>
        future_handle_;

    bool received_joint_states_ = false;
    bool received_result_ = false;
    bool result_ = false;
};

void saveTreeToXML(const BT::Tree& tree, const std::string& filename)
{
    std::string xml_content = BT::WriteTreeToXML(tree, true, true);
    std::ofstream out_file(filename);
    out_file << xml_content;
    out_file.close();
}


class PublishBoolAction : public BT::SyncActionNode
{
public:
  PublishBoolAction(const std::string &name, const BT::NodeConfiguration &config) : BT::SyncActionNode(name, config)
  {
    // Initialize the ROS 2 node
    node_ = rclcpp::Node::make_shared("publish_bool_action_node");
  }

  static BT::PortsList providedPorts()
  {
    return {
        BT::InputPort<bool>("value"),       // Input for the boolean value
        BT::InputPort<std::string>("topic") // Input for the topic name
    };
  }

  BT::NodeStatus tick() override
  {
    bool value;
    std::string topic;

    // Get the "value" input
    if (!getInput<bool>("value", value))
    {
      throw BT::RuntimeError("Missing required input [value]");
    }

    // Get the "topic" input
    if (!getInput<std::string>("topic", topic))
    {
      throw BT::RuntimeError("Missing required input [topic]");
    }

    // Create the publisher dynamically for the given topic
    rclcpp::QoS qos(rclcpp::QoSInitialization::from_rmw(rmw_qos_profile_default));
    qos.durability(rclcpp::DurabilityPolicy::TransientLocal); // Match Transient Local durability
    auto publisher = node_->create_publisher<std_msgs::msg::Bool>(topic, qos);

    // Publish the value to the topic
    std_msgs::msg::Bool msg;
    msg.data = value;
    publisher->publish(msg);

    RCLCPP_INFO(node_->get_logger(), "Published: %s to topic: %s", value ? "true" : "false", topic.c_str());
    return BT::NodeStatus::SUCCESS;
  }

private:
  rclcpp::Node::SharedPtr node_;
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<rclcpp::Node>("bt_moveit_joint_target_executor");
    BT::BehaviorTreeFactory factory;

    BT::RosNodeParams params;
    params.nh = node;
    params.default_port_value = "/execute_trajectory";

    factory.registerNodeType<GenerateAndExecuteTrajectory>("Move_to_position", params);
    factory.registerNodeType<PublishBoolAction>("PublishBool");


    auto tree = factory.createTreeFromFile("/home/benjamin/BT_ws/src/simple_bt/bt_fabricare.xml");
    //saveTreeToXML(tree, "/home/benjamin/BT_ws/src/simple_bt/bt_tree_saved.xml");

    BT::NodeStatus status = BT::NodeStatus::IDLE;
    rclcpp::Rate rate(10);
    while (rclcpp::ok() && status != BT::NodeStatus::SUCCESS)
    {
        status = tree.tickOnce();
        rclcpp::spin_some(node);
        rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}