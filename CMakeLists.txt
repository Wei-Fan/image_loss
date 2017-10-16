cmake_minimum_required(VERSION 2.8.3)
project(image_loss)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  sensor_msgs
  cv_bridge
  image_transport
  message_generation
  geometry_msgs
  ardrone_autonomy
)

find_package(OpenCV REQUIRED)

catkin_package(
  CATKIN_DEPENDS 
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  include
)

add_executable(image_loss 
  src/image_loss.cpp
)
target_link_libraries(image_loss
  ${catkin_LIBRARIES}
)
