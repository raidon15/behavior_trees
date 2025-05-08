#include <behaviortree_ros2/bt_action_node.hpp>
#include <rclcpp/rclcpp.hpp>
#include <control_msgs/action/follow_joint_trajectory.hpp>  // Replace with the correct action type
#include <chrono>
#include <moveit_msgs/action/execute_trajectory.hpp>
#include <filesystem>
#include <fstream>
#include "behaviortree_cpp/xml_parsing.h"  // Needed for XML export

class GenerateAndExecuteTrajectory : public BT::StatefulActionNode
{
public:
  GenerateAndExecuteTrajectory(const std::string& name, const BT::NodeConfig& config, const BT::RosNodeParams& params)
    : BT::StatefulActionNode(name, config), node_(params.nh)
  {
    // Create action client
    action_client_ = rclcpp_action::create_client<moveit_msgs::action::ExecuteTrajectory>(node_, "execute_trajectory");
  }

  static BT::PortsList providedPorts()
  {
    return {};
  }

  BT::NodeStatus onStart() override
  {
    // Example: Create a trajectory (you would normally get this from planning)
    moveit_msgs::msg::RobotTrajectory trajectory;
    trajectory.joint_trajectory.joint_names = { "joint_a1", "joint_a2", "joint_a3", "joint_a4",
                                                "joint_a5", "joint_a6", "joint_a7" };

    // Create a JointTrajectoryPoint message
    trajectory_msgs::msg::JointTrajectoryPoint initial_point;
    initial_point.time_from_start = rclcpp::Duration(1, 0);
    initial_point.positions = { 1.57, 0.0, 0.0, -1.57, 1.0, 0.0, 0.0 };
    trajectory.joint_trajectory.points.push_back(initial_point);

    trajectory_msgs::msg::JointTrajectoryPoint final_point;
    final_point.time_from_start = rclcpp::Duration(5, 0);
    final_point.positions = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    trajectory.joint_trajectory.points.push_back(final_point);

    // Send to action server
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

  void onHalted() override
  {
  }

private:
  rclcpp::Node::SharedPtr node_;
  rclcpp_action::Client<moveit_msgs::action::ExecuteTrajectory>::SharedPtr action_client_;
  std::shared_future<typename rclcpp_action::ClientGoalHandle<moveit_msgs::action::ExecuteTrajectory>::SharedPtr>
      future_handle_;

  std::vector<std::string> joint_names_;
  std::vector<double> target_joint_values_;
  double velocity_scaling_;
  double acceleration_scaling_;

  bool received_result_ = false;
  bool result_ = false;
};

class GenerateAndExecuteTrajectory2 : public BT::StatefulActionNode
{
public:
  GenerateAndExecuteTrajectory2(const std::string& name, const BT::NodeConfig& config, const BT::RosNodeParams& params)
    : BT::StatefulActionNode(name, config), node_(params.nh)
  {
    // Create action client
    action_client_ = rclcpp_action::create_client<moveit_msgs::action::ExecuteTrajectory>(node_, "execute_trajectory");
  }

  static BT::PortsList providedPorts()
  {
    return {};
  }

  BT::NodeStatus onStart() override
  {
    // Example: Create a trajectory (you would normally get this from planning)
    moveit_msgs::msg::RobotTrajectory trajectory;
    trajectory.joint_trajectory.joint_names = { "joint_a1", "joint_a2", "joint_a3", "joint_a4",
                                                "joint_a5", "joint_a6", "joint_a7" };

    // Create a JointTrajectoryPoint message
    trajectory_msgs::msg::JointTrajectoryPoint initial_point;
    initial_point.time_from_start = rclcpp::Duration(1, 0);
    initial_point.positions = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    trajectory.joint_trajectory.points.push_back(initial_point);

    trajectory_msgs::msg::JointTrajectoryPoint final_point;
    final_point.time_from_start = rclcpp::Duration(5, 0);
    final_point.positions = { 1.57, 0.0, 0.0, -1.57, 1.0, 0.0, 0.0 };
    trajectory.joint_trajectory.points.push_back(final_point);

    // Send to action server
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

  void onHalted() override
  {
  }

private:
  rclcpp::Node::SharedPtr node_;
  rclcpp_action::Client<moveit_msgs::action::ExecuteTrajectory>::SharedPtr action_client_;
  std::shared_future<typename rclcpp_action::ClientGoalHandle<moveit_msgs::action::ExecuteTrajectory>::SharedPtr>
      future_handle_;

  std::vector<std::string> joint_names_;
  std::vector<double> target_joint_values_;
  double velocity_scaling_;
  double acceleration_scaling_;

  bool received_result_ = false;
  bool result_ = false;
};

void saveTreeToXML(const BT::Tree& tree, const std::string& filename)
{
    // Modern way (BT.CPP 4.1+)
    
    // 2. Generate XML content
    std::string xml_content = BT::WriteTreeToXML(tree,true,true); // true = add TreeNodeModel
    
    // 3. Write to file
    std::ofstream out_file(filename);
    out_file << xml_content;
    out_file.close();
    
   }

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<rclcpp::Node>("bt_moveit_joint_target_executor");
  BT::BehaviorTreeFactory factory;

  // Provide the ROS node and the name of the action service
  BT::RosNodeParams params;
  params.nh = node;
  params.default_port_value = "/execute_trajectory";

  // Register the action node with the factory
  factory.registerNodeType<GenerateAndExecuteTrajectory>("MoveToJointTarget1", params);
  factory.registerNodeType<GenerateAndExecuteTrajectory2>("MoveToJointTarget2", params);

  auto tree =
      factory.createTreeFromFile("/home/benjamin/BT_ws/src/simple_bt/bt_tree.xml");  // Replace with your BT XML file

  saveTreeToXML(tree, "/home/benjamin/BT_ws/src/simple_bt/bt_tree_saved.xml");
  // Main loop
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