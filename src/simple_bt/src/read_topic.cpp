#include <behaviortree_ros2/bt_action_node.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>


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
        {   RCLCPP_INFO(node_->get_logger(), "Node is running...");
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



int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    
    BT::BehaviorTreeFactory factory;
    factory.registerNodeType<ReadJointStates>("ReadJointStates");
    
    // Load and run your tree
    auto tree = factory.createTreeFromFile("/home/benjamin/BT_ws/src/simple_bt/bt_read_topic.xml");

    auto node = rclcpp::Node::make_shared("bt_executor");
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);

    rclcpp::Rate loop_rate(10);  // Tick the tree at 10 Hz
    while (rclcpp::ok())
    {
        BT::NodeStatus status = tree.tickOnce();
        executor.spin_some();  // Process ROS 2 callbacks
        if (status != BT::NodeStatus::RUNNING)
        {
            break;  // Exit the loop if the tree is no longer running
        }
        loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}