#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

using namespace std::chrono_literals;

int main(int argc, char * argv[]) 	
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("publisher");
  auto publisher = node->create_publisher<geometry_msgs::msg::PoseStamped>("goal_pose", 10);
  geometry_msgs::msg::PoseStamped message;
  rclcpp::WallRate loop_rate(500ms);

  while (rclcpp::ok()) {
    message.header.stamp = rclcpp::Clock().now();
    message.header.frame_id = "map";

    message.pose.position.x = -1.0;
    message.pose.position.y = 3.0;
    message.pose.position.z = 0.0;

    message.pose.orientation.x = 0.0;
    message.pose.orientation.y = 0.0;
    message.pose.orientation.z = 0.0;
    message.pose.orientation.w = 1.0;

    publisher->publish(message);
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
  rclcpp::shutdown();
  return 0;
}



