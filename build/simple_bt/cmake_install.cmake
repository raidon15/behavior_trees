# Install script for directory: /home/benjamin/BT_ws/src/simple_bt

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/benjamin/BT_ws/install/simple_bt")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/bt_ros2" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/bt_ros2")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/bt_ros2"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/simple_bt" TYPE EXECUTABLE FILES "/home/benjamin/BT_ws/build/simple_bt/bt_ros2")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/bt_ros2" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/bt_ros2")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/bt_ros2"
         OLD_RPATH "/home/benjamin/ros2_ws/install/control_msgs/lib:/home/benjamin/ros2_humble/install/rclcpp_lifecycle/lib:/home/benjamin/ros2_humble/install/rcl_lifecycle/lib:/home/benjamin/ros2_humble/install/lifecycle_msgs/lib:/home/benjamin/ros2_humble/install/rclcpp_action/lib:/home/benjamin/ros2_humble/install/rclcpp/lib:/home/benjamin/ros2_humble/install/libstatistics_collector/lib:/home/benjamin/ros2_humble/install/rosgraph_msgs/lib:/home/benjamin/ros2_humble/install/statistics_msgs/lib:/home/benjamin/ros2_humble/install/rcl_action/lib:/home/benjamin/ros2_humble/install/rcl/lib:/home/benjamin/ros2_humble/install/rcl_interfaces/lib:/home/benjamin/ros2_humble/install/rcl_yaml_param_parser/lib:/home/benjamin/ros2_humble/install/libyaml_vendor/lib:/home/benjamin/ros2_humble/install/tracetools/lib:/home/benjamin/ros2_humble/install/rmw_implementation/lib:/home/benjamin/ros2_humble/install/rcl_logging_spdlog/lib:/home/benjamin/ros2_humble/install/rcl_logging_interface/lib:/home/benjamin/ros2_humble/install/ament_index_cpp/lib:/home/benjamin/ros2_humble/install/trajectory_msgs/lib:/home/benjamin/ros2_humble/install/action_msgs/lib:/home/benjamin/ros2_humble/install/unique_identifier_msgs/lib:/home/benjamin/ros2_humble/install/sensor_msgs/lib:/home/benjamin/ros2_humble/install/shape_msgs/lib:/home/benjamin/ros2_humble/install/geometry_msgs/lib:/home/benjamin/ros2_humble/install/std_msgs/lib:/home/benjamin/ros2_humble/install/builtin_interfaces/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_fastrtps_c/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_fastrtps_cpp/lib:/home/benjamin/ros2_humble/install/fastcdr/lib:/home/benjamin/ros2_humble/install/rmw/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_introspection_cpp/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_introspection_c/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_cpp/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_c/lib:/home/benjamin/ros2_humble/install/rosidl_runtime_c/lib:/home/benjamin/ros2_humble/install/rcpputils/lib:/home/benjamin/ros2_humble/install/rcutils/lib:/opt/ros/humble/lib:/home/benjamin/BT_ws/install/btcpp_ros2_interfaces/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/bt_ros2")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/read_topic" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/read_topic")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/read_topic"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/simple_bt" TYPE EXECUTABLE FILES "/home/benjamin/BT_ws/build/simple_bt/read_topic")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/read_topic" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/read_topic")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/read_topic"
         OLD_RPATH "/home/benjamin/ros2_humble/install/sensor_msgs/lib:/home/benjamin/ros2_humble/install/rclcpp_lifecycle/lib:/home/benjamin/ros2_humble/install/rcl_lifecycle/lib:/home/benjamin/ros2_humble/install/lifecycle_msgs/lib:/home/benjamin/ros2_humble/install/rclcpp_action/lib:/home/benjamin/ros2_humble/install/rclcpp/lib:/home/benjamin/ros2_humble/install/libstatistics_collector/lib:/home/benjamin/ros2_humble/install/rosgraph_msgs/lib:/home/benjamin/ros2_humble/install/statistics_msgs/lib:/home/benjamin/ros2_humble/install/rcl_action/lib:/home/benjamin/ros2_humble/install/rcl/lib:/home/benjamin/ros2_humble/install/rcl_interfaces/lib:/home/benjamin/ros2_humble/install/rcl_yaml_param_parser/lib:/home/benjamin/ros2_humble/install/libyaml_vendor/lib:/home/benjamin/ros2_humble/install/tracetools/lib:/home/benjamin/ros2_humble/install/rmw_implementation/lib:/home/benjamin/ros2_humble/install/rcl_logging_spdlog/lib:/home/benjamin/ros2_humble/install/rcl_logging_interface/lib:/home/benjamin/ros2_humble/install/ament_index_cpp/lib:/home/benjamin/ros2_humble/install/action_msgs/lib:/home/benjamin/ros2_humble/install/unique_identifier_msgs/lib:/home/benjamin/ros2_humble/install/geometry_msgs/lib:/home/benjamin/ros2_humble/install/std_msgs/lib:/home/benjamin/ros2_humble/install/builtin_interfaces/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_fastrtps_c/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_fastrtps_cpp/lib:/home/benjamin/ros2_humble/install/fastcdr/lib:/home/benjamin/ros2_humble/install/rmw/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_introspection_cpp/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_introspection_c/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_cpp/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_c/lib:/home/benjamin/ros2_humble/install/rosidl_runtime_c/lib:/home/benjamin/ros2_humble/install/rcpputils/lib:/home/benjamin/ros2_humble/install/rcutils/lib:/opt/ros/humble/lib:/home/benjamin/BT_ws/install/btcpp_ros2_interfaces/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/read_topic")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/combined_nodes" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/combined_nodes")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/combined_nodes"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/simple_bt" TYPE EXECUTABLE FILES "/home/benjamin/BT_ws/build/simple_bt/combined_nodes")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/combined_nodes" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/combined_nodes")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/combined_nodes"
         OLD_RPATH "/home/benjamin/ros2_ws/install/control_msgs/lib:/home/benjamin/ros2_humble/install/rclcpp_lifecycle/lib:/home/benjamin/ros2_humble/install/rcl_lifecycle/lib:/home/benjamin/ros2_humble/install/lifecycle_msgs/lib:/home/benjamin/ros2_humble/install/rclcpp_action/lib:/home/benjamin/ros2_humble/install/rclcpp/lib:/home/benjamin/ros2_humble/install/libstatistics_collector/lib:/home/benjamin/ros2_humble/install/rosgraph_msgs/lib:/home/benjamin/ros2_humble/install/statistics_msgs/lib:/home/benjamin/ros2_humble/install/rcl_action/lib:/home/benjamin/ros2_humble/install/rcl/lib:/home/benjamin/ros2_humble/install/rcl_interfaces/lib:/home/benjamin/ros2_humble/install/rcl_yaml_param_parser/lib:/home/benjamin/ros2_humble/install/libyaml_vendor/lib:/home/benjamin/ros2_humble/install/tracetools/lib:/home/benjamin/ros2_humble/install/rmw_implementation/lib:/home/benjamin/ros2_humble/install/rcl_logging_spdlog/lib:/home/benjamin/ros2_humble/install/rcl_logging_interface/lib:/home/benjamin/ros2_humble/install/ament_index_cpp/lib:/home/benjamin/ros2_humble/install/trajectory_msgs/lib:/home/benjamin/ros2_humble/install/sensor_msgs/lib:/home/benjamin/ros2_humble/install/action_msgs/lib:/home/benjamin/ros2_humble/install/unique_identifier_msgs/lib:/home/benjamin/ros2_humble/install/shape_msgs/lib:/home/benjamin/ros2_humble/install/geometry_msgs/lib:/home/benjamin/ros2_humble/install/std_msgs/lib:/home/benjamin/ros2_humble/install/builtin_interfaces/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_fastrtps_c/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_fastrtps_cpp/lib:/home/benjamin/ros2_humble/install/fastcdr/lib:/home/benjamin/ros2_humble/install/rmw/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_introspection_cpp/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_introspection_c/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_cpp/lib:/home/benjamin/ros2_humble/install/rosidl_typesupport_c/lib:/home/benjamin/ros2_humble/install/rosidl_runtime_c/lib:/home/benjamin/ros2_humble/install/rcpputils/lib:/home/benjamin/ros2_humble/install/rcutils/lib:/opt/ros/humble/lib:/home/benjamin/BT_ws/install/btcpp_ros2_interfaces/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/simple_bt/combined_nodes")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/benjamin/BT_ws/build/simple_bt/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
