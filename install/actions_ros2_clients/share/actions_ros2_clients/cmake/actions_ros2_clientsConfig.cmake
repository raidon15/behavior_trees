# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_actions_ros2_clients_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED actions_ros2_clients_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(actions_ros2_clients_FOUND FALSE)
  elseif(NOT actions_ros2_clients_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(actions_ros2_clients_FOUND FALSE)
  endif()
  return()
endif()
set(_actions_ros2_clients_CONFIG_INCLUDED TRUE)

# output package information
if(NOT actions_ros2_clients_FIND_QUIETLY)
  message(STATUS "Found actions_ros2_clients: 0.1.0 (${actions_ros2_clients_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'actions_ros2_clients' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${actions_ros2_clients_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(actions_ros2_clients_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${actions_ros2_clients_DIR}/${_extra}")
endforeach()
