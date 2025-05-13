#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include <behaviortree_ros2/bt_action_node.hpp>

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

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  // Create the ROS 2 node and BehaviorTree factory
  auto node = rclcpp::Node::make_shared("bt_publish_bool_node");
  BT::BehaviorTreeFactory factory;
  factory.registerNodeType<PublishBoolAction>("PublishBool");

  // Load the behavior tree from an XML file
  auto tree = factory.createTreeFromText(
      R"(<root BTCPP_format="4" main_tree_to_execute="MainTree">
    <BehaviorTree ID="MainTree">
        <Sequence>
            <PublishBool value="false" topic="/fabricare/electrovalve_1" />
            <PublishBool value="true" topic="/fabricare/electrovalve_2" />
        </Sequence>
    </BehaviorTree>
</root>)");

  // Main loop to tick the tree
  rclcpp::spin_some(node);
  tree.tickOnce();

  rclcpp::shutdown();
  return 0;
}