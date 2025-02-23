#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

int sum = 0;
std::shared_ptr<rclcpp::Publisher<std_msgs::msg::Int32>> publisher;
void callback(const std_msgs::msg::Int32::SharedPtr msg){
	sum+= msg->data;
	std_msgs::msg::Int32 out_msg;
	out_msg.data = sum;
	publisher -> publish(out_msg);
	
}

int main(int argc, char * argv[]){
	rclcpp::init(argc,argv);
	auto node = rclcpp::Node::make_shared("sumador");
	publisher = node -> create_publisher<std_msgs::msg::Int32>("sum",10);
	auto subscriber = node -> create_subscription<std_msgs::msg::Int32>("numero",10,callback);
	rclcpp::spin(node);
	rclcpp::shutdown();
	return 0;
}

