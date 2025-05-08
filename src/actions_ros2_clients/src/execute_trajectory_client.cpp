#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <moveit_msgs/action/execute_trajectory.hpp>

class ExecuteTrajectoryActionClient : public rclcpp::Node
{
public:
    using ExecuteTrajectory = moveit_msgs::action::ExecuteTrajectory;
    using GoalHandle = rclcpp_action::ClientGoalHandle<ExecuteTrajectory>;

    ExecuteTrajectoryActionClient() 
        : Node("execute_trajectory_action_client")
    {
        // Create action client
        this->client_ptr_ = rclcpp_action::create_client<ExecuteTrajectory>(
            this,
            "/execute_trajectory");  // Default MoveGroup action name
        
        RCLCPP_INFO(this->get_logger(), "ExecuteTrajectory action client created");
    }

    // Send goal function
    void send_goal(const moveit_msgs::msg::RobotTrajectory& trajectory)
    {
        using namespace std::placeholders;

        if (!client_ptr_->wait_for_action_server(std::chrono::seconds(10))) {
            RCLCPP_ERROR(this->get_logger(), "Action server not available after waiting");
            return;
        }

        auto goal_msg = ExecuteTrajectory::Goal();
        goal_msg.trajectory = trajectory;

        RCLCPP_INFO(this->get_logger(), "Sending trajectory goal");

        auto send_goal_options = rclcpp_action::Client<ExecuteTrajectory>::SendGoalOptions();
        send_goal_options.goal_response_callback =
            std::bind(&ExecuteTrajectoryActionClient::goal_response_callback, this, _1);
        send_goal_options.feedback_callback =
            std::bind(&ExecuteTrajectoryActionClient::feedback_callback, this, _1, _2);
        send_goal_options.result_callback =
            std::bind(&ExecuteTrajectoryActionClient::result_callback, this, _1);

        client_ptr_->async_send_goal(goal_msg, send_goal_options);
    }

private:
    rclcpp_action::Client<ExecuteTrajectory>::SharedPtr client_ptr_;

    // Goal response callback
    void goal_response_callback(const GoalHandle::SharedPtr & goal_handle)
    {
        if (!goal_handle) {
            RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server");
        } else {
            RCLCPP_INFO(this->get_logger(), 
                        "Goal accepted by server, waiting for result");
        }
    }

    // Feedback callback
    void feedback_callback(
        GoalHandle::SharedPtr,
        const std::shared_ptr<const ExecuteTrajectory::Feedback> feedback)
    {
        // Process feedback if needed
        RCLCPP_INFO(this->get_logger(), "Execution progress...");
    }

    // Result callback
    void result_callback(const GoalHandle::WrappedResult & result)
    {
        switch (result.code) {
            case rclcpp_action::ResultCode::SUCCEEDED:
                RCLCPP_INFO(this->get_logger(), "Trajectory execution succeeded");
                break;
            case rclcpp_action::ResultCode::ABORTED:
                RCLCPP_ERROR(this->get_logger(), "Trajectory execution aborted");
                break;
            case rclcpp_action::ResultCode::CANCELED:
                RCLCPP_ERROR(this->get_logger(), "Trajectory execution canceled");
                break;
            default:
                RCLCPP_ERROR(this->get_logger(), "Unknown result code");
                break;
        }
    }
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    
    auto node = std::make_shared<ExecuteTrajectoryActionClient>();
    
    // Example: Create a trajectory (you would normally get this from planning)
    moveit_msgs::msg::RobotTrajectory trajectory;
    trajectory.joint_trajectory.joint_names = {"joint_a1", "joint_a2", "joint_a3", "joint_a4", "joint_a5", "joint_a6", "joint_a7"};


    // Create a JointTrajectoryPoint message
    trajectory_msgs::msg::JointTrajectoryPoint initial_point;
    initial_point.time_from_start = rclcpp::Duration(1,0);
    initial_point.positions = {1.57, 0.0, 0.0, -1.57, 1.0, 0.0, 0.0};
    trajectory.joint_trajectory.points.push_back(initial_point);

    trajectory_msgs::msg::JointTrajectoryPoint final_point;
    final_point.time_from_start = rclcpp::Duration(5,0);
    final_point.positions = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    trajectory.joint_trajectory.points.push_back(final_point);

    
    // ... populate trajectory with your planned path ...
    
    node->send_goal(trajectory);
    
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}