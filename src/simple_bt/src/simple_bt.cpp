#include <iostream>
#include <chrono>
#include "behaviortree_cpp_v3/bt_factory.h"
#include "behaviortree_cpp_v3/action_node.h"

using namespace std::chrono_literals;

// node class
class Move :public BT::SyncActionNode{
    public:
        explicit Move(const std::string &name) : BT::SyncActionNode(name, {})
        {        
        } 
        
        
        BT::NodeStatus tick() override
        {
            std::cout << "Moving Robot " << std::endl;
            std::this_thread::sleep_for(5s);
            return BT::NodeStatus::SUCCESS;
        }
}
;

int main()
{   
    BT::BehaviorTreeFactory factory;

    factory.registerNodeType<Move>("Move");

    auto tree = factory.createTreeFromFile("/home/benjamin/BT_ws/src/simple_bt/bt_tree.xml");

    
    tree.tickRoot();




    return 0;
}