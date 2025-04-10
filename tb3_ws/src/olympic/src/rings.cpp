#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"



int main(int argc, char * argv[])
{
 rclcpp::init(argc, argv);
 auto node = rclcpp::Node::make_shared("rings");
 
 auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);
 
 geometry_msgs::msg::Twist vel;
 
 while (rclcpp::ok()) {
   vel.linear.x = 1.0;
   vel.angular.z = 1.0;
   publisher->publish(vel);
   rclcpp::spin_some(node);
  }
  
 rclcpp::shutdown();
 return 0;
 
}

