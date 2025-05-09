#ifndef SIMPLE_BT_GENERATE_TRAJECTORY_HPP
#define SIMPLE_BT_GENERATE_TRAJECTORY_HPP

#include <behaviortree_ros2/bt_action_node.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <moveit_msgs/action/execute_trajectory.hpp>
#include <vector>
#include <string>
#include <future>

class GenerateAndExecuteTrajectory : public BT::StatefulActionNode
{
public:
    GenerateAndExecuteTrajectory(const std::string& name, const BT::NodeConfig& config, const BT::RosNodeParams& params);

    static BT::PortsList providedPorts();

    BT::NodeStatus onStart() override;
    BT::NodeStatus onRunning() override;
    void onHalted() override;

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

std::vector<double> parseInputToVector(const std::string& input);

#endif // SIMPLE_BT_GENERATE_TRAJECTORY_HPP