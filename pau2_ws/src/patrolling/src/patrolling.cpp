#include <chrono>
#include <vector>
#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

using namespace std::chrono_literals;


double pose_x; 
double pose_y; 

const double goal_tolerance = 0.2;


void topic_callback(const nav_msgs::msg::Odometry::SharedPtr msg)
{
 	pose_x = msg->pose.pose.position.x;
	pose_y = msg->pose.pose.position.y;


   std::cout << "Received Odometry: pose_x = " << pose_x << ", pose_y = " << pose_y << std::endl;
}


int main(int argc, char * argv[]) 	
{
  rclcpp::init(argc, argv);
  
  auto node = rclcpp::Node::make_shared("patrolling_node");
  auto subscription = node->create_subscription<nav_msgs::msg::Odometry>("odom", 10, topic_callback);
  auto publisher = node->create_publisher<geometry_msgs::msg::PoseStamped>("goal_pose", 10);
  geometry_msgs::msg::PoseStamped message;
  rclcpp::WallRate loop_rate(500ms);

 
  while (rclcpp::ok()) {

    double goal_x = -1.0;
    double goal_y = 3.0;

    //message.header.stamp = rclcpp::Clock().now();
    //message.header.frame_id = "map3";

    message.pose.position.x = -1.0;
    message.pose.position.y = 3.0;
    message.pose.position.z = 0.0;


    message.pose.orientation.x = 0.0;
    message.pose.orientation.y = 0.0;
    message.pose.orientation.z = 0.0;
    message.pose.orientation.w = 1.0;

    publisher->publish(message);

    std::cout << "Published Goal Pose: x = " << message.pose.position.x << ", y = " << message.pose.position.y << std::endl;

    if ( std::abs(pose_x - goal_x) < goal_tolerance && std::abs(pose_y - goal_y) < goal_tolerance) {
      std::cout << "Funciona" << std::endl;
      break;
    }

    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
  rclcpp::shutdown();
  return 0;
}



