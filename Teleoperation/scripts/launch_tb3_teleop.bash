#!/bin/bash
source /opt/ros/foxy/setup.bash
source install/setup.bash
export ROS_LOCALHOST_ONLY=1
export TURTLEBOT3_MODEL=burger
ros2 run turtlebot3_teleop teleop_keyboard


 
