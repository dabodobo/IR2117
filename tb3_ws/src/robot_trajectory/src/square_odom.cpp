#include <chrono>  
#include "rclcpp/rclcpp.hpp" 
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "cmath"
#include "map"


float quat(float x, float y, float z, float w){
	float siny_cosp = 2 *(w*z + x*y);
	float cosy_cosp = 1 - 2*(y*y +z*z);
	double yaw = std::atan2(siny_cosp, cosy_cosp); 
	return yaw;
}

float dist(float base_x,float base_y, float x, float y){
	float dist_x = x - base_x;
	float dist_y = y - base_y;
	return sqrt(dist_x*dist_x + dist_y*dist_y);
}
float angle_dist(float base_angle, float current){
	
	    float dist = current - base_angle;
	    
	    // Normalizar la diferencia al rango [-π, π]
	    if (dist > M_PI) {
		dist -= 2 * M_PI;  // Si es mayor que π, restamos 2π
	    } else if (dist < -M_PI) {
		dist += 2 * M_PI;  // Si es menor que -π, sumamos 2π
	    }
	    return dist;
	
}

using namespace std::chrono_literals;


std::map<std::string,float> ini_position;
std::map<std::string,float> pos;
float angle_x;
float angle_y;
float angle_z;
float angle_w;
bool first_read = true;



void topic_callback(const nav_msgs::msg::Odometry msg){
	angle_x = msg.pose.pose.orientation.x;
	angle_y = msg.pose.pose.orientation.y;
	angle_z = msg.pose.pose.orientation.z;
	angle_w = msg.pose.pose.orientation.w;
	if (first_read){
	    ini_position["x"] = msg.pose.pose.position.x;
	    ini_position["y"] = msg.pose.pose.position.y;
	    ini_position["angle"] = quat(angle_x,angle_y,angle_z,angle_w);
	}
	first_read = false;
	
	
	
	pos["x"] =  msg.pose.pose.position.x;
	pos ["y"] = msg.pose.pose.position.y;
	pos["angle"] = quat(angle_x,angle_y,angle_z,angle_w);
	
	
	
	std::cout << "Position x: " << pos["x"] << std:: endl;
	std::cout << "Position y: " << pos["y"] << std:: endl;
	std::cout << "Angle : " << pos["angle"] << std::endl;
	std::cout << "Distance : " << dist(ini_position["x"],ini_position["y"],pos["x"],pos["y"]) << std::endl;
	std::cout << "Dif angle: " << angle_dist(ini_position["angle"],pos["angle"]) << std::endl;
	std::cout << "- - - - - - - - - - - " << std::endl << std::endl;
	
	

	
	
}


int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv); 
    auto node = rclcpp::Node::make_shared("node_position"); //creo el nodo odom
    auto subscription = node->create_subscription<nav_msgs::msg::Odometry>("odom", 10, topic_callback); //creo la suscripcion a odom y hago call_back
    auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
    
    rclcpp::WallRate loop_rate(10ms);
    geometry_msgs::msg::Twist message;
    std::cout << "Empiezo !! " << std::endl;
    
    for(int i = 0; i < 4; i++){
    
    first_read = true;
    std::cout << "Avanzo !! " << std::endl  << std::endl;
    while(rclcpp::ok() &&  dist(ini_position["x"],ini_position["y"],pos["x"],pos["y"]) < 1){
    	message.linear.x = 0.1;
    	publisher -> publish(message);
    	rclcpp::spin_some(node);
    	loop_rate.sleep(); 
    	
    }
    first_read = true;
    message.linear.x = 0.0;
    publisher -> publish(message);
    
      
    
    std::cout << "Giro !! " << std::endl  << std::endl;
     while(rclcpp::ok() && angle_dist(ini_position["angle"],pos["angle"]) < M_PI/2){
     	message.angular.z = 0.13;
     	publisher -> publish(message);
     	rclcpp::spin_some(node);
     	loop_rate.sleep(); 
     	
     
     }
     
     first_read = true;
     message.angular.z = 0.0;
     publisher -> publish(message);
     

    rclcpp::spin_some(node);
    
    }
    message.angular.z = 0.0;
    for(int i = 0; i < 100;i++){
    	 publisher -> publish(message);
    	 rclcpp::spin_some(node);
    	 loop_rate.sleep(); 
    }
    rclcpp::shutdown(); 
    return 0;
}

