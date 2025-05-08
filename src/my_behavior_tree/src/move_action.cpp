#include <behaviortree_cpp_v3/bt_factory.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <rclcpp/rclcpp.hpp>

using namespace BT;

class MoveArm : public SyncActionNode
{
public:
    MoveArm(const std::string& name, const NodeConfiguration& config)
        : SyncActionNode(name, config) {}

    static PortsList providedPorts()
    {
        return { InputPort<std::vector<double>>("target_position") };
    }

    NodeStatus tick() override
    {
        std::vector<double> target_position;
        if (!getInput("target_position", target_position))
        {
            throw RuntimeError("missing required input [target_position]");
        }

        // Initialize MoveGroup
        auto node = rclcpp::Node::make_shared("move_arm_node");
        rclcpp::executors::SingleThreadedExecutor executor;
        executor.add_node(node);
        std::thread([&executor]() { executor.spin(); }).detach();

        moveit::planning_interface::MoveGroupInterface move_group(node, "iiwa_arm");

        // Set target position
        move_group.setJointValueTarget(target_position);

        // Plan and execute
        moveit::planning_interface::MoveGroupInterface::Plan plan;
        bool success = (move_group.plan(plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

        if (success)
        {
            move_group.move();
            return NodeStatus::SUCCESS;
        }
        else
        {
            return NodeStatus::FAILURE;
        }
    }
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);

    BehaviorTreeFactory factory;
    factory.registerNodeType<MoveArm>("MoveArm");

    auto tree = factory.createTreeFromFile("/home/benjamin/BT_ws/src/my_behavior_tree/my_behavior_tree.xml");

    while (rclcpp::ok())
    {
        tree.tickRoot();
        rclcpp::spin_some(rclcpp::Node::make_shared("dummy_node"));
    }

    rclcpp::shutdown();
    return 0;
}