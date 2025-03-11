#include "rclcpp/rclcpp.hpp" 
#include "geometry_msgs/msg/twist.hpp" 
#include "sensor_msgs/msg/laser_scan.hpp"
#include <vector>
#include <chrono>
#include <cmath>



using namespace std::chrono_literals;

float min_distance = 1.0;
float min;



void callback(const sensor_msgs::msg::LaserScan& sensor){
	std::vector<float> vec1;
	min = sensor.ranges[0];
	
	for(int i = 45; i < 135; i++){
	
		if (sensor.ranges[i] < min){
		
			min = sensor.ranges[i];
		}

	}
	
}

int main(int argc, char * argv[]){
	rclcpp::init(argc,argv); 
	auto node = rclcpp::Node::make_shared("sensor_node"); 
	auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel",10); 
	auto subscriber = node->create_subscription<sensor_msgs::msg::LaserScan>("/scan",10,callback);
	
	rclcpp::WallRate loop_rate(10ms);
	
	geometry_msgs::msg::Twist vel;
	
	
	while(rclcpp::ok()){ 
		vel.linear.x = min > min_distance ? 1.0 : 0.0;
		publisher->publish(vel);
		rclcpp::spin_some(node);
		loop_rate.sleep();
		
	
	}
	rclcpp::shutdown();
	return 0;


}
