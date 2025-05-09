#include "simple_bt/generate_trajectory.hpp"
#include <sstream>
#include <algorithm>

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

GenerateAndExecuteTrajectory::GenerateAndExecuteTrajectory(const std::string& name, const BT::NodeConfig& config, const BT::RosNodeParams& params)
    : BT::StatefulActionNode(name, config), node_(params.nh)
{
    action_client_ = rclcpp_action::create_client<moveit_msgs::action::ExecuteTrajectory>(node_, "execute_trajectory");

    subscription_ = node_->create_subscription<sensor_msgs::msg::JointState>(
        "/joint_states", 10,
        [this](const sensor_msgs::msg::JointState::SharedPtr msg) {
            latest_joint_states_ = *msg;
            received_joint_states_ = true;
        });
}

BT::PortsList GenerateAndExecuteTrajectory::providedPorts()
{
    return {
        BT::InputPort<std::string>("final_positions")
    };
}

BT::NodeStatus GenerateAndExecuteTrajectory::onStart()
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

BT::NodeStatus GenerateAndExecuteTrajectory::onRunning()
{
    if (received_result_)
    {
        setOutput("success", result_);
        return result_ ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
    }
    return BT::NodeStatus::RUNNING;
}

void GenerateAndExecuteTrajectory::onHalted() {}