#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include <chrono>

using namespace std::chrono_literals;
int n = 0;
int main(int argc, char* argv[]){
  rclcpp::init(argc,argv);
  auto node = rclcpp::Node::make_shared("square");
  auto publisher = node -> create_publisher<geometry_msgs::msg::Twist>("/cmd_vel",10);
  geometry_msgs::msg::Twist vel;
  rclcpp::WallRate loop_rate(10ms);
  
  while(rclcpp::ok() && n <= 1000){
    vel.linear.x = 0.1;
    publisher -> publish(vel);
    rclcpp::spin_some(node);
    loop_rate.sleep();
    n++;
  
  }
  vel.linear.x = 0.0;
    publisher -> publish(vel);
    rclcpp::spin_some(node);
  rclcpp::shutdown();
  return 0;
  
}
