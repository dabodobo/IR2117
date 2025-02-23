#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/int32.hpp"



std::shared_ptr<rclcpp::Publisher<std_msgs::msg::Float32>> publisher;

float valores;
int count;

void topic_callback(const std_msgs::msg::Int32::SharedPtr msg){
	valores+=msg->data;
	count++;
	std_msgs::msg::Float32 out_msg;
	out_msg.data = valores / count;
	publisher -> publish(out_msg);
}

int main(int argc, char* argv[]){
	rclcpp::init(argc,argv);
	auto node = rclcpp::Node::make_shared("media");
	publisher = node -> create_publisher<std_msgs::msg::Float32>("media",10);
	auto subscriber = node -> create_subscription<std_msgs::msg::Int32>("number",10,topic_callback);
	rclcpp::spin(node);
	return 0;
}
