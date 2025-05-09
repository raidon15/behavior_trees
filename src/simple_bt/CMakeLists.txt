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
find_package(moveit_msgs REQUIRED)      # Add rclcpp for ROS 2 nodes


# Add executable
add_executable(bt_ros2 src/bt_ros2.cpp)
add_executable(read_topic src/read_topic.cpp)


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

install(TARGETS
  bt_ros2
  read_topic
  DESTINATION lib/${PROJECT_NAME}
)
