#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <chrono>

using namespace std::chrono_literals;

int main(int argc,char * argv[]){

	rclcpp::init(argc,argv);
	
	auto node = rclcpp::Node::make_shared("speaker");
	auto publisher = node->create_publisher<std_msgs::msg::String>("talk",10);
	std_msgs::msg::String msg;
	msg.data = "Chochonet99";
	rclcpp::WallRate loop_rate(1000ms);
	
	while(rclcpp::ok()){
		publisher -> publish(msg);
		rclcpp::spin_some(node);
		loop_rate.sleep();
	
	}
	rclcpp::shutdown();
	return 0;
}
