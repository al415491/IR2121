#include <chrono>
#include <iostream>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

using namespace std::chrono_literals;

// Variables globales para almacenar la posición actual del robot
double pose_x = 0.0;
double pose_y = 0.0;
double error_x = 1.9;
double error_y = 0.6;

void topic_callback(const nav_msgs::msg::Odometry::SharedPtr msg)
{
    pose_x = msg->pose.pose.position.x;
    pose_y = msg->pose.pose.position.y;
    std::cout << "Posición actual: (" << pose_x << ", " << pose_y << ")\n";
}

bool is_goal_reached(double pose_x, double pose_y, double goal_x, double goal_y, double threshold)
{
    double distance = std::sqrt(std::pow(pose_x - (goal_x-error_x), 2) + std::pow(pose_y - (goal_y - error_y), 2));
    std::cout << "Distancia al objetivo: " << distance << "\n";
    return distance < threshold;
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("programa1_node");
    auto subscription = node->create_subscription<nav_msgs::msg::Odometry>("odom", 10, topic_callback);
    auto publisher = node->create_publisher<geometry_msgs::msg::PoseStamped>("goal_pose", 10);

    geometry_msgs::msg::PoseStamped message;
    rclcpp::WallRate loop_rate(500ms);

    // Establecer la pose objetivo
    
    std::vector<std::pair<double, double>> goals = {
        {5.26, -2.84},
        {1.25, 3.57},
        {-6.35, -1.17},
        {-2.65, 3.19},
        {-1.38, -1.14}
    };
    
    
    double threshold = 0.55;
    for (auto& goal : goals)
    {
        geometry_msgs::msg::PoseStamped message;
        double goal_x = goal.first + error_x;
        double goal_y = goal.second + error_y; 

    message.pose.position.x = goal_x;
    message.pose.position.y = goal_y;

    publisher->publish(message);

    while (rclcpp::ok() && !is_goal_reached(pose_x, pose_y, goal_x, goal_y, threshold)) 
    {
        rclcpp::spin_some(node);
        loop_rate.sleep();
    }
    }
	
	std::cout << "El robot ha alcanzado su objetivo. Finalizando el programa." << std::endl;
    rclcpp::shutdown();
    return 0;
}
