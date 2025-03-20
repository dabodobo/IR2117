#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "example_interfaces/msg/bool.hpp"
#include <iostream>

std::shared_ptr <rclcpp::Publisher<example_interfaces::msg::Bool> > publisher;


void topic_callback(const sensor_msgs::msg::LaserScan& sensor){ 
	example_interfaces::msg::Bool msg;
	msg.data = false;
	publisher -> publish(msg);
}

int main(int argc, char * argv[]){
	rclcpp::init(argc,argv);
	auto node = rclcpp::Node::make_shared("detector2");
	publisher = node -> create_publisher<example_interfaces::msg::Bool>("/obstacles",10);
	auto subscription = node->create_subscription<sensor_msgs::msg::LaserScan>("/scan",10,topic_callback); 
	rclcpp::spin(node); 
	rclcpp::shutdown();
	return 0;


}
