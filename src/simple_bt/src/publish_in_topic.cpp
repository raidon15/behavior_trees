#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include <behaviortree_ros2/bt_action_node.hpp>

class PublishBoolAction : public BT::SyncActionNode
{
public:
  PublishBoolAction(const std::string& name, const BT::NodeConfiguration& config) : BT::SyncActionNode(name, config)
  {
    // Initialize the ROS 2 node and publisher with compatible QoS
    node_ = rclcpp::Node::make_shared("publish_bool_action_node");

    // Set QoS to match the subscriber's QoS
    rclcpp::QoS qos(rclcpp::QoSInitialization::from_rmw(rmw_qos_profile_default));
    qos.durability(rclcpp::DurabilityPolicy::TransientLocal);  // Match Transient Local durability

    publisher_ = node_->create_publisher<std_msgs::msg::Bool>("/fabricare/electrovalve_1", qos);
  }

  static BT::PortsList providedPorts()
  {
    return { BT::InputPort<bool>("value") };
  }

  BT::NodeStatus tick() override
  {
    bool value;
    if (!getInput<bool>("value", value))
    {
      throw BT::RuntimeError("Missing required input [value]");
    }

    // Publish the value to the topic
    std_msgs::msg::Bool msg;
    msg.data = value;
    publisher_->publish(msg);

    RCLCPP_INFO(node_->get_logger(), "Published: %s", value ? "true" : "false");
    return BT::NodeStatus::SUCCESS;
  }

private:
  rclcpp::Node::SharedPtr node_;
  rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr publisher_;
};

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);

  // Create the ROS 2 node and BehaviorTree factory
  auto node = rclcpp::Node::make_shared("bt_publish_bool_node");
  BT::BehaviorTreeFactory factory;
  factory.registerNodeType<PublishBoolAction>("PublishBool");

  // Load the behavior tree from an XML file
  auto tree = factory.createTreeFromText(
      R"(
        <root main_tree_to_execute="MainTree">
            <BehaviorTree ID="MainTree">
                <PublishBool value="true" />
            </BehaviorTree>
        </root>
        )");

  // Main loop to tick the tree
  rclcpp::spin_some(node);
  tree.tickOnce();

  rclcpp::shutdown();
  return 0;
}