#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "example_interfaces/msg/bool.hpp"
#include <iostream>

std::shared_ptr <rclcpp::Publisher<example_interfaces::msg::Bool> > publisher;
double person_angle_min, person_angle_max, person_distance_min, person_distance_max;

void topic_callback(const sensor_msgs::msg::LaserScan& sensor){ 
	example_interfaces::msg::Bool msg;
	msg.data = false;
	float angle = sensor.angle_min;
	for (float range : sensor.ranges){
		if(angle > M_PI) angle -= 2*M_PI;
		if((angle >= person_angle_min) && (angle <= person_angle_max)){
			if( range >= person_distance_min && range <= person_distance_max) 
				msg.data = true; //está dentro de los 2 valores min y max
		}
		angle += sensor.angle_increment;
	
	
	}
	publisher -> publish(msg);
}

int main(int argc, char * argv[]){
	rclcpp::init(argc,argv);
        
	auto node = rclcpp::Node::make_shared("detector");
	auto subscription = node->create_subscription<sensor_msgs::msg::LaserScan>("/scan",10,topic_callback); 
	publisher = node -> create_publisher<example_interfaces::msg::Bool>("person",10);
	
	node -> declare_parameter("person_angle_min", -M_PI/12);
	node -> declare_parameter("person_angle_max", M_PI/12);
	node -> declare_parameter("person_distance_min", 0.5);
	node -> declare_parameter("person_distance_max", 1.3);
	
	person_angle_min = node->get_parameter("person_angle_min").get_parameter_value().get<double>();
	person_angle_max = node->get_parameter("person_angle_max").get_parameter_value().get<double>();
	person_distance_min = node->get_parameter("person_distance_min").get_parameter_value().get<double>();
	person_distance_max = node->get_parameter("person_distance_max").get_parameter_value().get<double>();
	
	rclcpp::spin(node); 
	rclcpp::shutdown();
	return 0;


}
