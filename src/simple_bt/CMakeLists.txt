cmake_minimum_required(VERSION 3.10)

# Project name
project(simple_bt)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Threads REQUIRED)
find_package(behaviortree_ros2 REQUIRED)
find_package(control_msgs REQUIRED)       # Add control_msgs
find_package(trajectory_msgs REQUIRED)    # Add trajectory_msgs
find_package(rclcpp REQUIRED)        
find_package(moveit_msgs REQUIRED)        # Add moveit_msgs
find_package(sensor_msgs REQUIRED)        # Add sensor_msgs
find_package(std_msgs REQUIRED)           # Add std_msgs

# Add executables
add_executable(bt_ros2 src/bt_ros2.cpp)
add_executable(read_topic src/read_topic.cpp)
add_executable(combined_nodes src/combined_nodes.cpp)
add_executable(publish_in_topic src/publish_in_topic.cpp)  # Add publish_in_topic node
add_executable(fabricare_node src/fabricare.cpp)       # Add fabricare node

# Link dependencies
ament_target_dependencies(bt_ros2
  behaviortree_ros2
  control_msgs
  trajectory_msgs
  rclcpp
  moveit_msgs
)

ament_target_dependencies(read_topic
  behaviortree_ros2
  sensor_msgs
  rclcpp
)

ament_target_dependencies(combined_nodes
  behaviortree_ros2
  control_msgs
  trajectory_msgs
  sensor_msgs
  rclcpp
  moveit_msgs
)

ament_target_dependencies(publish_in_topic  # Dependencies for publish_in_topic
  behaviortree_ros2
  std_msgs
  rclcpp
)

ament_target_dependencies(fabricare_node  # Dependencies for fabricare_node
  behaviortree_ros2
  control_msgs
  std_msgs
  moveit_msgs
  trajectory_msgs
  rclcpp
)

# Install targets
install(TARGETS
  bt_ros2
  read_topic
  combined_nodes
  publish_in_topic  # Install publish_in_topic
  fabricare_node    # Install fabricare_node
  DESTINATION lib/${PROJECT_NAME}
)
