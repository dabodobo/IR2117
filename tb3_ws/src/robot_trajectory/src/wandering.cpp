#include "rclcpp/rclcpp.hpp" 
#include "geometry_msgs/msg/twist.hpp" 
#include "sensor_msgs/msg/laser_scan.hpp"
#include <vector>
#include <chrono>
#include <cmath>



using namespace std::chrono_literals;

float min_distance = 0.5;
float min;
float min_izq;
float min_der;
bool turn_left = false;
bool turn_right = false;



void callback(const sensor_msgs::msg::LaserScan& sensor){
	min = sensor.ranges[0];
	
	for(int i = 0; i < 60; i++){
	
		if (sensor.ranges[i] < min && (sensor.ranges[i] != 0)){
		
			min = sensor.ranges[i];
		}

	}
	for(int i = 300; i < 359; i++){
	
		if (sensor.ranges[i] < min && (sensor.ranges[i] != 0)){
		
			min = sensor.ranges[i];
		}

	}
	

	min_izq = sensor.ranges[0];
	for(int i = 0; i < 10; i++){

		if (sensor.ranges[i] < min_izq && (sensor.ranges[i] != 0)){
		
			min_izq = sensor.ranges[i];
		}

	}

	min_der = sensor.ranges[350];

	for(int i = 350; i < 360; i++){
		if (sensor.ranges[i] < min_der && (sensor.ranges[i] != 0)){

			min_der = sensor.ranges[i];

		}

	}
	std::cout << "[0..9]: " <<min_izq << " // [350,359]" << min_der << " // min::  " << min <<  std::endl; 
	
	
}

void avanzar (geometry_msgs::msg::Twist& vel){
	vel.linear.x = 0.1;
	vel.angular.z = 0.0;
	//std::cout << "AVANZ0 !! " << std::endl;
}

void girar_izq(geometry_msgs::msg::Twist& vel){
	vel.linear.x = 0.0;
	vel.angular.z = 0.1;
	//std::cout << "IZQUIERDA !! " << std::endl;
}

void girar_der(geometry_msgs::msg::Twist& vel){
	vel.linear.x = 0.0;
	vel.angular.z = -0.1;
	//std::cout << "DERECHA !! " << std::endl;
}

void stop(geometry_msgs::msg::Twist& vel){
	vel.linear.x = 0.0;
	vel.angular.z = 0.0;
}

int main(int argc, char * argv[]){
	rclcpp::init(argc,argv); 
	auto node = rclcpp::Node::make_shared("sensor_node"); 
	
	rclcpp::QoS qos_profile(10);
        qos_profile.best_effort();  // Usamos una QoS fiable para el publicador
	auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel",10); 
	auto subscriber = node->create_subscription<sensor_msgs::msg::LaserScan>("/scan",qos_profile,callback);
	
	rclcpp::WallRate loop_rate(10ms);
	
	geometry_msgs::msg::Twist vel;
	
	
	while(rclcpp::ok()){ 
		if(turn_left == false && turn_right == false){
			avanzar(vel);
			if(min < min_distance && min > 0){
				if(min_izq > min_der){
					turn_left = true;
				}
				else{
					turn_right = true;
				}
			}
		}
		
		if (turn_left){
			girar_izq(vel);
			
			if(min > min_distance){
				turn_left = false;
			}
		}
		
		if (turn_right){
			girar_der(vel);
			
			if(min > min_distance){
				turn_right = false;
			}
		}
		
		
		publisher->publish(vel);
		rclcpp::spin_some(node);
		loop_rate.sleep();
		
	
	}
	stop(vel);
    	for(int i = 0; i < 100;i++){
    	 publisher -> publish(vel);
    	 rclcpp::spin_some(node);
    	 loop_rate.sleep(); 
    	}
	std::cout << "PROCESS FINISHED " << std::endl;
	rclcpp::shutdown();
	return 0;


}
