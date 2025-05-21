#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;
int n = 0;

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

int main(int argc, char* argv[]){
  int n = 0;
  rclcpp::init(argc,argv);
  auto node = rclcpp::Node::make_shared("square");
  auto publisher = node -> create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",10);
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
