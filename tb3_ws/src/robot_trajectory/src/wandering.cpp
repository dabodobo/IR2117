#include "rclcpp/rclcpp.hpp" 
#include "geometry_msgs/msg/twist" 
#include "sensor_msgs/msg/laserscan"
#include <chrono>
#include "cmath"


using namespace std::chrono_literals;

float min_distance = 0.5

void girar(const geometry_msgs::msg::Twist msg){
	
}

void collision(const sensor_msgs::msg::LaserScan& msg){
	for (float i : msg.ranges){
		if(i < min_distance){
			vel.angular.z 
		}
	}


}
int main(int argc, char * argv[]){
	rclcpp::init(argc,argv); 
	auto node = rclcpp::Node::make_shared("sensor_node"); 
	auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel",10); 
	auto subscriber = node->create_subscription<sensor_msgs::msg::LaserScan>("/scan",);
	
	rclcpp::WallRate loop_rate(10ms);
	
	geometry_msgs::msg::Twist vel;
	sensor_msgs::msg::LaserScan sensor;
	
	sensor.angle_min = (1/4) * M_PI;
	sensor.angle_max = (3/4) * M_PI;
	
	
	while(rclcpp::ok()){ 
		vel.linear.x = 0.1;
		publisher->publish(vel);
		spin_some(node);
		loop_rate.sleep();
		
	
	}
	rclcpp::shutdown();
	return 0;


}
