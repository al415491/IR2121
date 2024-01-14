#include <chrono>
#include <iostream>
#include <cmath>
#include <vector>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_with_covariance_stamped.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

using namespace std::chrono_literals;

class RobotNavigator : public rclcpp::Node
{
public:
    RobotNavigator() : Node("programa1_node")
    {
        subscription_ = this->create_subscription<geometry_msgs::msg::PoseWithCovarianceStamped>(
            "amcl_pose", 10, std::bind(&RobotNavigator::amcl_pose_callback, this, std::placeholders::_1));
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
        goal_reached_ = false;
        current_goal_index_ = 0;

        publish_next_goal();
    }

private:
    void amcl_pose_callback(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg)
    {
        double pose_x = msg->pose.pose.position.x;
        double pose_y = msg->pose.pose.position.y;
        std::cout << "Posición actual: (" << pose_x << ", " << pose_y << ")\n";

        if (is_goal_reached(pose_x, pose_y, goals_[current_goal_index_].first, goals_[current_goal_index_].second))
        {
            std::cout << "Objetivo alcanzado: (" << goals_[current_goal_index_].first << ", " << goals_[current_goal_index_].second << ")\n";
            goal_reached_ = true;
            if (current_goal_index_ < goals_.size() - 1)
            {
                current_goal_index_++;
                publish_next_goal();
            }
            else
            {
                std::cout << "Todos los objetivos alcanzados. Finalizando el programa.\n";
                rclcpp::shutdown();
            }
        }
    }

    bool is_goal_reached(double pose_x, double pose_y, double goal_x, double goal_y)
    {
        double adjusted_goal_x = goal_x + error_x_;
        double adjusted_goal_y = goal_y + error_y_;
        double distance = std::sqrt(std::pow(pose_x - adjusted_goal_x, 2) + std::pow(pose_y - adjusted_goal_y, 2));
        return distance < threshold_;
    }

    void publish_next_goal()
    {
        goal_reached_ = false;
        auto message = geometry_msgs::msg::PoseStamped();
        message.pose.position.x = goals_[current_goal_index_].first + error_x_;
        message.pose.position.y = goals_[current_goal_index_].second + error_y_;
        publisher_->publish(message);
    }

    rclcpp::Subscription<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr subscription_;
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr publisher_;
	std::vector<std::pair<double, double>> goals_;
	double error_x_;
	double error_y_;
	double threshold_;
	bool goal_reached_;
	size_t current_goal_index_;

};

int main(int argc, char *argv[])
{
	rclcpp::init(argc, argv);
	auto node = std::make_shared<RobotNavigator>();
	rclcpp::spin(node);
	rclcpp::shutdown();
	return 0;
}

