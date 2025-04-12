#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;


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

void show_data(const nav_msgs::msg::Odometry data ){
  float x = data.pose.pose.position.x;
  float y = data.pose.pose.position.y;
  std:: cout << "x: " << x << std::endl;
  std:: cout << "y: " << y << std::endl;
  std:: cout << std::endl << std::endl;

}

int main(int argc, char* argv[]){
  int n = 0;
  rclcpp::init(argc,argv);
  auto node = rclcpp::Node::make_shared("square");
  auto publisher = node -> create_publisher<geometry_msgs::msg::Twist>("/cmd_vel",10);
  auto subscriber = node -> create_subscription<nav_msgs::msg::Odometry>("/odom",10, show_data);
  geometry_msgs::msg::Twist vel;
  rclcpp::WallRate loop_rate(10ms);
  
  for(int i = 0; i < 4; i++){
  
    while(rclcpp::ok() && n <= 1000){
      avanzar(vel);
      publisher -> publish(vel);
      rclcpp::spin_some(node);
      loop_rate.sleep();
      n++;
    
    }
      stop(vel);
      publisher -> publish(vel);
      rclcpp::spin_some(node);
      n = 0;
      
    while(rclcpp::ok() && n <= 1570){
      girar(vel);
      publisher -> publish(vel);
      rclcpp::spin_some(node);
      loop_rate.sleep();
      n++;
    
    }
    
      stop(vel);
      publisher -> publish(vel);
      rclcpp::spin_some(node);
      n = 0;
    
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
