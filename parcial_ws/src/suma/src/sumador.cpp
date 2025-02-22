#include "rclcpp/rclcpp.hpp"
#include <iostream>
#include "std_msgs/msg/int32.hpp"

int sum = 0;
void callback(const std_msgs::msg::Int32::SharedPtr msg){
	sum+= msg->data;
	std::cout << sum << std::endl;
	
}

int main(int argc, char * argv[]){
	rclcpp::init(argc,argv);
	auto node = rclcpp::Node::make_shared("sumador");
	auto subscriber = node -> create_subscription<std_msgs::msg::Int32>("numero",10,callback);
	rclcpp::spin(node);
	rclcpp::shutdown();
	return 0;
}

