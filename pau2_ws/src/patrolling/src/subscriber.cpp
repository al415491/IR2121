#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include <iostream>
#include <cmath> //M_PI


double xp; 
double yp; 
double orientation_x; 
double orientation_y;
double orientation_z; 
double orientation_w;
double angle;
double siny_cosp;
double cospy_cosp;

void topic_callback(const nav_msgs::msg::Odometry::SharedPtr msg)
{
 	xp = msg->pose.pose.position.x;
	yp = msg->pose.pose.position.y;
	orientation_x = msg->pose.pose.orientation.x;
    orientation_y = msg->pose.pose.orientation.y;
    orientation_z = msg->pose.pose.orientation.z;
    orientation_w = msg->pose.pose.orientation.w;
    siny_cosp = 2* (orientation_w*orientation_z + orientation_x*orientation_y);
    cospy_cosp = 1 - 2*(orientation_y*orientation_y + orientation_z*orientation_z);
    angle = atan2(siny_cosp, cospy_cosp);
    if (angle < 0) {
    	angle += 2 * M_PI;
    }
    
	
	std::cout << "Pos x: " << xp << std::endl;
	std::cout << "Pos y: " << yp << std::endl;
	std::cout << "Angle in radians: " << angle << std::endl;
	std::cout << "----------" << std::endl;
}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("subscriber");
  auto subscription = node->create_subscription<nav_msgs::msg::Odometry>("/odom", 10, topic_callback);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}


