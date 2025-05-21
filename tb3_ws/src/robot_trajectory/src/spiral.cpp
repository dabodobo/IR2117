#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <chrono>
#include <map>
#include <cmath>
#include <iostream>
using namespace std::chrono_literals;
std::map<std::string, float> position;
bool first_read = true;
float v = 0.1;
bool circle_completed = false;


void girar(geometry_msgs::msg::Twist& vel,double distance,double time){
      double angle = vel.angular.z * time;
      vel.linear.x = (angular*distance*angle)/(2*M_PI);
      
}

void stop(geometry_msgs::msg::Twist& vel){
      vel.linear.x = 0.0;
      vel.angular.z = 0.0;
}




int main (int argc, char* argv[]){
  rclcpp::init(argc,argv);
  auto node = rclcpp::Node::make_shared("spiral");
  auto publisher = node -> create_publisher<geometry_msgs::msg::Twist>("/cmd_vel",10);
  
  // Declaración de parámetros
  
  node -> declare_parameter("angular_speed",0.5);
  double angular_speed = node -> get_parameter("angular_speed").get_parameter_value().get<double>();
  
  node -> declare_parameter("distance_between_loops",1.0);
  double distance_between_loops = node -> get_parameter("distance_between_loops").get_parameter_value().get<double>();
  
  node -> declare_parameter("number_of_loops",3);
  int number_of_loops = node -> get_parameter("number_of_loops").get_parameter_value().get<int>();
  
  rclcpp::WallRate loop_rate(500ms);
  
  
  geometry_msgs::msg::Twist vel;
  
  std::cout << "Empiezo . . .  "<< std::endl;
  
  for(int vuelta = 1; vuelta <= number_of_loops; vuelta++){
    int n = 0 
    while(rclcpp::ok() && n <= M_PI/(0.5*angular_speed)){
    girar(vel,distance_between_loops,time);
    publisher -> publish(vel);
    rclcpp::spin_some(node);
    loop_rate.sleep();
    }
    stop(vel);
    publisher -> publish(vel);
    rclcpp::spin_some(node);
    loop_rate.sleep();

    
  }
  std::cout << "Acabo . . . "<< std::endl;
  stop(vel);
  publisher -> publish(vel);
  rclcpp::spin_some(node);
  loop_rate.sleep();
  
  rclcpp::shutdown();
  return 0;
  

}
