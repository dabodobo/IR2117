#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"

float lin_vel = 1.0;


int main(int argc, char * argv[])
{
 rclcpp::init(argc, argv);
 auto node = rclcpp::Node::make_shared("rings");
 
 auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);
 
 geometry_msgs::msg::Twist vel;
 
 node -> declare_parameter("radius",1.0); // necesitamos declarar previamente el parametro en el nodo antes de obtenerlo en la linea siguiente
 float radio =  node -> get_parameter("radius").get_parameter_value().get<double>();
 
 while (rclcpp::ok()) {
 
   vel.linear.x = lin_vel;
   vel.angular.z = lin_vel / radio;
   publisher->publish(vel);
   rclcpp::spin_some(node);
   
  }
  
 rclcpp::shutdown();
 return 0;
 
}

