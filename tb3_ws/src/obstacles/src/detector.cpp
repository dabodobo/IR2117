#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "example_interfaces/msg/bool.hpp"
#include <iostream>

std::shared_ptr <rclcpp::Publisher<example_interfaces::msg::Bool> > publisher;
double obs_angle_min, obs_angle_max, obs_threshold;

void topic_callback(const sensor_msgs::msg::LaserScan& sensor){ 
	example_interfaces::msg::Bool msg;
	msg.data = false;
	float angle = sensor.angle_min;
	for (float range : sensor.ranges){
		if(angle > M_PI) angle -= 2*M_PI;
		if((angle >= obs_angle_min) && (angle <= obs_angle_max)){
			if(range <= obs_threshold)
				msg.data = true;
		}
		angle += sensor.angle_increment;
	
	
	}
	publisher -> publish(msg);
}

int main(int argc, char * argv[]){
	rclcpp::init(argc,argv);
	auto node = rclcpp::Node::make_shared("detector");
	auto subscription = node->create_subscription<sensor_msgs::msg::LaserScan>("/scan",10,topic_callback); 
	publisher = node -> create_publisher<example_interfaces::msg::Bool>("obstacle",10);
	
	node -> declare_parameter("obs_angle_min", -M_PI/8);
	node -> declare_parameter("obs_angle_max", M_PI/8);
	node -> declare_parameter("obs_threshold", 1.0);
	
	obs_angle_min = node->get_parameter("obs_angle_min").get_parameter_value().get<double>();
	obs_angle_max = node->get_parameter("obs_angle_max").get_parameter_value().get<double>();
	obs_threshold = node->get_parameter("obs_threshold").get_parameter_value().get<double>();
	
	rclcpp::spin(node); 
	rclcpp::shutdown();
	return 0;


}
