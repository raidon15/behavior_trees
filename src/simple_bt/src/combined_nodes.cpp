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

// Utility function to parse input strings into vectors
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

// Class: ReadJointStates
class ReadJointStates : public BT::StatefulActionNode
{
public:
    ReadJointStates(const std::string& name, const BT::NodeConfiguration& config)
        : StatefulActionNode(name, config)
    {
        node_ = rclcpp::Node::make_shared("read_joint_states_bt_node");
        subscription_ = node_->create_subscription<sensor_msgs::msg::JointState>(
            "/joint_states", 10,
            [this](const sensor_msgs::msg::JointState::SharedPtr msg) {
                latest_joint_states_ = *msg;
                received_data_ = true;
                RCLCPP_INFO(node_->get_logger(), "reading joint states");
            });
    }

    static BT::PortsList providedPorts()
    {
        return { 
            BT::OutputPort<std::vector<std::string>>("joint_names"),
            BT::OutputPort<std::vector<double>>("positions"),
            BT::OutputPort<std::vector<double>>("velocities"),
            BT::OutputPort<std::vector<double>>("efforts")
        };
    }

    BT::NodeStatus onStart() override
    {
        received_data_ = false;
        RCLCPP_INFO(node_->get_logger(), "Node is running...");
        return BT::NodeStatus::RUNNING;
    }

    BT::NodeStatus onRunning() override
    {
        rclcpp::spin_some(node_);
        if (received_data_)
        {
            setOutput("joint_names", latest_joint_states_.name);
            setOutput("positions", latest_joint_states_.position);
            setOutput("velocities", latest_joint_states_.velocity);
            setOutput("efforts", latest_joint_states_.effort);

            for (size_t i = 0; i < latest_joint_states_.name.size(); ++i)
            {
                RCLCPP_INFO(node_->get_logger(), "%s: %f", 
                            latest_joint_states_.name[i].c_str(), 
                            latest_joint_states_.position[i]);
            }

            return BT::NodeStatus::SUCCESS;
        }
        
        return BT::NodeStatus::RUNNING;
    }

    void onHalted() override {}

private:
    rclcpp::Node::SharedPtr node_;
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr subscription_;
    sensor_msgs::msg::JointState latest_joint_states_;
    bool received_data_ = false;
};

// Class: GenerateAndExecuteTrajectory
class GenerateAndExecuteTrajectory : public BT::StatefulActionNode
{
public:
    GenerateAndExecuteTrajectory(const std::string& name, const BT::NodeConfig& config, const BT::RosNodeParams& params)
        : BT::StatefulActionNode(name, config), node_(params.nh)
    {
        action_client_ = rclcpp_action::create_client<moveit_msgs::action::ExecuteTrajectory>(node_, "execute_trajectory");
    }

    static BT::PortsList providedPorts()
    {
        return {
            BT::OutputPort<std::vector<double>>("initial_positions"),
            BT::InputPort<std::string>("final_positions")
        };
    }

    BT::NodeStatus onStart() override
    {
        std::vector<double> initial_positions;
        std::string final_positions_str;

        if (!getInput("initial_positions", initial_positions))
        {
            RCLCPP_ERROR(node_->get_logger(), "Missing input [initial_positions]");
            return BT::NodeStatus::FAILURE;
        }

        if (initial_positions.empty())
        {
            RCLCPP_ERROR(node_->get_logger(), "Input [initial_positions] is empty");
            return BT::NodeStatus::FAILURE;
        }

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

        moveit_msgs::msg::RobotTrajectory trajectory;
        trajectory.joint_trajectory.joint_names = { "joint_a1", "joint_a2", "joint_a3", "joint_a4",
                                                    "joint_a5", "joint_a6", "joint_a7" };

        trajectory_msgs::msg::JointTrajectoryPoint initial_point;
        initial_point.time_from_start = rclcpp::Duration(1, 0);
        initial_point.positions = initial_positions;
        trajectory.joint_trajectory.points.push_back(initial_point);

        trajectory_msgs::msg::JointTrajectoryPoint final_point;
        final_point.time_from_start = rclcpp::Duration(5, 0);
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
    std::shared_future<typename rclcpp_action::ClientGoalHandle<moveit_msgs::action::ExecuteTrajectory>::SharedPtr>
        future_handle_;

    bool received_result_ = false;
    bool result_ = false;
};

// Function to save the tree to an XML file
void saveTreeToXML(const BT::Tree& tree, const std::string& filename)
{
    std::string xml_content = BT::WriteTreeToXML(tree, true, true);
    std::ofstream out_file(filename);
    out_file << xml_content;
    out_file.close();
}

// Main function
int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<rclcpp::Node>("bt_combined_executor");
    BT::BehaviorTreeFactory factory;

    // Register nodes
    factory.registerNodeType<ReadJointStates>("ReadJointStates");
    BT::RosNodeParams params;
    params.nh = node;
    params.default_port_value = "/execute_trajectory";
    factory.registerNodeType<GenerateAndExecuteTrajectory>("MoveToJointTarget", params);

    auto tree = factory.createTreeFromFile("/home/benjamin/BT_ws/src/simple_bt/bt_tree.xml");
    saveTreeToXML(tree, "/home/benjamin/BT_ws/src/simple_bt/bt_tree_saved.xml");

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