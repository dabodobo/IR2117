#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <iostream>


void callback (const std_msgs::msg::String msg){
	std::cout << "He escuchado " << msg.data << std::endl;

}

int main(int argc,char * argv[]){

	rclcpp::init(argc,argv);
	
	auto node = rclcpp::Node::make_shared("listener");
	auto subscriber = node->create_subscription<std_msgs::msg::String>("talk",10,callback);
	rclcpp::spin(node);
	rclcpp::shutdown();
	return 0;
}
