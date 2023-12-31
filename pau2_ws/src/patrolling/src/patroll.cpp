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

void topic_callback(const nav_msgs::msg::Odometry::SharedPtr msg)
{
    pose_x = msg->pose.pose.position.x;
    pose_y = msg->pose.pose.position.y;
    std::cout << "Posición actual: (" << pose_x << ", " << pose_y << ")\n";
}

bool is_goal_reached(double pose_x, double pose_y, double goal_x, double goal_y, double threshold)
{
    double distance = std::sqrt(std::pow(pose_x - (goal_x-1.9), 2) + std::pow(pose_y - (goal_y - 0.6), 2));
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
    double goal_x = -2.6 + 1.9 ;
    double goal_y = 3.0 + 0.6;
    double threshold = 0.2; 

    message.pose.position.x = goal_x;
    message.pose.position.y = goal_y;

    publisher->publish(message);

    while (rclcpp::ok())
    {
        rclcpp::spin_some(node);

        if (is_goal_reached(pose_x, pose_y, goal_x, goal_y, threshold))
        {
            std::cout << "El robot ha alcanzado su objetivo. Finalizando el programa." << std::endl;
            break;
        }

        loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}
