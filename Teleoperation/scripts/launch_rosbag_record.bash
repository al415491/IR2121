#!/bin/bash
source /opt/ros/foxy/setup.bash
export ROS_LOCALHOST_ONLY=1
ros2 bag record /clock /odom /tf /tf_static

