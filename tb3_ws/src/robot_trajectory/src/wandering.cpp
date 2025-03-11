#include "rclcpp/rclcpp.hpp" 
#include "geometry_msgs/msg/twist.hpp" 
#include "sensor_msgs/msg/laser_scan.hpp"
#include <vector>
#include <chrono>
#include <cmath>



using namespace std::chrono_literals;

float min_distance = 0.5;
std::vector<int> data = {0,90,180,270};


void callback(const sensor_msgs::msg::LaserScan& sensor){
	for (float i : data){
		std::cout<< "[" << i << "] = " << sensor.ranges[i] << "  - -   ";
	}
	std::cout << std::endl;


}

int main(int argc, char * argv[]){
	rclcpp::init(argc,argv); 
	auto node = rclcpp::Node::make_shared("sensor_node"); 
	auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel",10); 
	auto subscriber = node->create_subscription<sensor_msgs::msg::LaserScan>("/scan",10,callback);
	
	rclcpp::WallRate loop_rate(10ms);
	
	geometry_msgs::msg::Twist vel;
	
	
	
	
	
	while(rclcpp::ok()){ 
		vel.linear.x = 0.1;
		publisher->publish(vel);
		spin_some(node);
		loop_rate.sleep();
		
	
	}
	rclcpp::shutdown();
	return 0;


}
