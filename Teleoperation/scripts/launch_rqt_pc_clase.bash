#!/bin/bash
source /opt/ros/foxy/setup.bash
export ROS_LOCALHOST_ONLY=1
sudo apt update
sudo apt install ros-foxy-rqt-common-plugins
rqt --force-discover 

 
