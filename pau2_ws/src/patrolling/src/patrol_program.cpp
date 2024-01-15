#include <chrono>
#include <iostream>
#include <cmath>
#include <vector>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"

using namespace std::chrono_literals;

class RobotNavigator : public rclcpp::Node
{
public:
    RobotNavigator()
        : Node("programa1_node"),
          tf_buffer_(std::make_shared<rclcpp::Clock>()),
          tf_listener_(tf_buffer_)
    {
        publisher_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("goal_pose", 10);

        goals_ = {
            {5.26, -2.84},
            {1.25, 3.57},
            {-6.35, -1.17},
            {-2.65, 3.19},
            {-1.38, -1.14}
        };

        error_x_ = 1.9;
        error_y_ = 0.6;
        threshold_ = 0.55;
        current_goal_index_ = 0;

        publish_next_goal();
    }

    bool check_goal_reached()
    {
        try
        {
            auto transform_stamped = tf_buffer_.lookupTransform("map", "base_link", tf2::TimePointZero);
            double pose_x = transform_stamped.transform.translation.x;
            double pose_y = transform_stamped.transform.translation.y;

            if (is_goal_reached(pose_x, pose_y, goals_[current_goal_index_].first, goals_[current_goal_index_].second))
            {
                std::cout << "Objetivo alcanzado: (" << goals_[current_goal_index_].first << ", " << goals_[current_goal_index_].second << ")\n";
				if (current_goal_index_ < goals_.size() - 1)
				{
					current_goal_index_++;
					publish_next_goal();
					return false;
				} else
				{
					std::cout << "Todos los objetivos alcanzados. Finalizando el programa.\n";
					return true;
				}
			}
		}
		catch (tf2::TransformException &ex)
			{
			RCLCPP_WARN(this->get_logger(), "No se pudo obtener la transformación: %s", ex.what());
			}
		return false;
	}

private:
	bool is_goal_reached(double pose_x, double pose_y, double goal_x, double goal_y)
	{
		double adjusted_goal_x = goal_x + error_x_;
		double adjusted_goal_y = goal_y + error_y_;
		double distance = std::sqrt(std::pow(pose_x - adjusted_goal_x, 2) + std::pow(pose_y - adjusted_goal_y, 2));
		return distance < threshold_;
	}	
	void publish_next_goal()
	{
    	auto message = geometry_msgs::msg::PoseStamped();
    	message.pose.position.x = goals_[current_goal_index_].first + error_x_;
    	message.pose.position.y = goals_[current_goal_index_].second + error_y_;
    	publisher_->publish(message);
	}

	rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr publisher_;
	std::vector<std::pair<double, double>> goals_;
	double error_x_;
	double error_y_;
	double threshold_;
	size_t current_goal_index_;
	tf2_ros::Buffer tf_buffer_;
	tf2_ros::TransformListener tf_listener_;
	
};

int main(int argc, char *argv[])
{
	rclcpp::init(argc, argv);
	auto node = std::make_shared<RobotNavigator>();
	rclcpp::Rate rate(10);
	while (rclcpp::ok())
	{
    	if (node->check_goal_reached()) {
        	break;
    	}
    	rclcpp::spin_some(node);
    	rate.sleep();
	}

	rclcpp::shutdown();
	return 0;
}





