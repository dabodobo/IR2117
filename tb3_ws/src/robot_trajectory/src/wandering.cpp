#include "rclcpp/rclcpp.hpp" 
#include "geometry_msgs/msg/twist.hpp" 
#include "sensor_msgs/msg/laser_scan.hpp"
#include <vector>
#include <chrono>
#include <cmath>



using namespace std::chrono_literals;

float min_distance = 1.0;
float min;
float min_izq;
float min_der;



void callback(const sensor_msgs::msg::LaserScan& sensor){
	min = sensor.ranges[0];
	
	for(int i = 45; i < 135; i++){
	
		if (sensor.ranges[i] < min){
		
			min = sensor.ranges[i];
		}

	}
	

	min_izq = sensor.ranges[0];
	for(int i = 0; i < 10; i++){

		if (sensor.ranges[i] < min_izq){
		
			min_izq = sensor.ranges[i];
		}

	}

	min_der = sensor.ranges[350];

	for(int i = 350; i < 360; i++){
		if (sensor.ranges[i] < min_der){

			min_der = sensor.ranges[i];

		}

	}
	std::cout << "[0..9]: " <<min_izq << " // [350,359]" << min_der << " // min::  " << min <<  std::endl; 
	
	
}

void avanzar (geometry_msgs::msg::Twist& vel){
	vel.linear.x = 0.1;
	vel.angular.z = 0.0;
}

void girar_izq(geometry_msgs::msg::Twist& vel){
	vel.linear.x = 0.0;
	vel.angular.z = 0.1;
}

void girar_der(geometry_msgs::msg::Twist& vel){
	vel.linear.x = 0.0;
	vel.angular.z = -0.1;
}

int main(int argc, char * argv[]){
	rclcpp::init(argc,argv); 
	auto node = rclcpp::Node::make_shared("sensor_node"); 
	auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel",10); 
	auto subscriber = node->create_subscription<sensor_msgs::msg::LaserScan>("/scan",10,callback);
	
	rclcpp::WallRate loop_rate(10ms);
	
	geometry_msgs::msg::Twist vel;
	
	
	while(rclcpp::ok()){ 
		if(min > min_distance){
			avanzar(vel);
		}
		else if (min_izq > min_der){
			girar_izq(vel);
		}
		else {
			girar_der(vel);
		}
		
		publisher->publish(vel);
		rclcpp::spin_some(node);
		loop_rate.sleep();
		
	
	}
	rclcpp::shutdown();
	return 0;


}
