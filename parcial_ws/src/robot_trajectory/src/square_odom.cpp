#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include <chrono>
#include <iostream>
#include <map>
#include <cmath>
using namespace std::chrono_literals;
bool first_read = true;

float angle_x;
float angle_y;
float angle_z;
float angle_w;
std::map<std::string,float> pos;

void avanzar (geometry_msgs::msg::Twist& vel){
  vel.linear.x  = 0.1;
  vel.angular.z = 0.0;
}
void stop (geometry_msgs::msg::Twist& vel){
  vel.linear.x  = 0.0;
  vel.angular.z = 0.0;
}
void girar (geometry_msgs::msg::Twist& vel){
  vel.linear.x  = 0.0;
  vel.angular.z = 0.1;
}

float quat(float x, float y, float z, float w){
	float siny_cosp = 2 *(w*z + x*y);
	float cosy_cosp = 1 - 2*(y*y +z*z);
	double yaw = std::atan2(siny_cosp, cosy_cosp); 
	return yaw;
}

float dist(float x1, float x2, float y1, float y2){
      float dist_x = x1 - x2;
      float dist_y = y1 - y2;
      return sqrt(pow(dist_x,2) + pow(dist_y,2));
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
void show_data(const nav_msgs::msg::Odometry data ){
  if(first_read){
  pos["ini_x"]= data.pose.pose.position.x;
  pos["ini_y"]= data.pose.pose.position.y;
  pos["ini_angle"]= quat(data.pose.pose.orientation.x,data.pose.pose.orientation.y,data.pose.pose.orientation.z,data.pose.pose.orientation.w);
  std::cout << "Position x: " << pos["x"] << std:: endl;
  std::cout << "Position y: " << pos["y"] << std:: endl;
  std::cout << "Angle : " << pos["angle"] << std::endl;
  
  std::cout << "- - - - - - - - - - - " << std::endl << std::endl;
   std::cout << "- - - - - - - - - - - " << std::endl << std::endl;
  
  }
  first_read = false;
  pos["x"] = data.pose.pose.position.x;
  pos["y"] =data.pose.pose.position.y;
  pos["angle"] = quat(data.pose.pose.orientation.x,data.pose.pose.orientation.y,data.pose.pose.orientation.z,data.pose.pose.orientation.w);
  std::cout << "Position x: " << pos["x"] << std:: endl;
  std::cout << "Position y: " << pos["y"] << std:: endl;
  std::cout << "Angle : " << pos["angle"] << std::endl;
  std::cout << "Distance : " << dist(pos["ini_x"],pos["x"],pos["ini_y"],pos["y"]) << std::endl;
  std::cout << "Dif angle: " << angle_dist(pos["ini_angle"],pos["angle"]) << std::endl;
  std::cout << "- - - - - - - - - - - " << std::endl << std::endl;
  
  

}

int main(int argc, char* argv[]){
  
  rclcpp::init(argc,argv);
  auto node = rclcpp::Node::make_shared("square");
  auto publisher = node -> create_publisher<geometry_msgs::msg::Twist>("/cmd_vel",10);
  auto subscriber = node -> create_subscription<nav_msgs::msg::Odometry>("/odom",10, show_data);
  geometry_msgs::msg::Twist vel;
  rclcpp::WallRate loop_rate(10ms);
  
  for(int i = 0; i < 4; i++){
    first_read = true;
     std::cout << "Avanzo !! " << std::endl  << std::endl;
    while(rclcpp::ok() && dist(pos["ini_x"],pos["x"],pos["ini_y"],pos["y"]) < 1){
      avanzar(vel);
      publisher -> publish(vel);
      rclcpp::spin_some(node);
      loop_rate.sleep();
    
    }
      first_read = true;
      stop(vel);
      publisher -> publish(vel);
      loop_rate.sleep();
     
    std::cout << "Giro !! " << std::endl  << std::endl;
    while(rclcpp::ok() && angle_dist(pos["ini_angle"],pos["angle"]) < M_PI/2){
      girar(vel);
      publisher -> publish(vel);
      rclcpp::spin_some(node);
      loop_rate.sleep();
    
    }
      first_read = true;
      stop(vel);
      publisher -> publish(vel);
      loop_rate.sleep();
      
     
    
  }
  std::cout << "Acabado ... "<< std::endl;
  for(int i = 0; i < 100; i++){
    stop(vel);
    publisher -> publish(vel);
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
   std::cout << "Parado ... "<< std::endl;
  rclcpp::shutdown();
  return 0;
  
}
