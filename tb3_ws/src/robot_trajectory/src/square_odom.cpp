#include <chrono>  
#include "rclcpp/rclcpp.hpp" 
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "cmath"

using namespace std::chrono_literals;

void topic_callback(const nav_msgs::msg::Odometry msg){
	auto x = msg.pose.pose.position.x;
	auto y = msg.pose.pose.position.y;
	std::cout 
}


int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv); 
    auto node = rclcpp::Node::make_shared("node_odom"); //creo el nodo odom
    
    auto subscription = node->create_subscription<nav_msgs::msg::Odometry>("odom", 10, topic_callback); //creo la suscripcion a odom u call_back

   
    rclcpp::shutdown(); 
    return 0;
}

