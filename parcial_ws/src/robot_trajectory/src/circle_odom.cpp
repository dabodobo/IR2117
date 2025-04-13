#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <chrono>
#include <map>
#include <cmath>
using namespace std::chrono_literals;
std::map<std::string, float> position;
bool first_read = true;
float v = 0.1;
bool circle_completed = false;


void dist(float x1, float x2, float y1, float y2){
  float dist_x = x1-x2;
  float dist_y = y1 -y2;
  float distance = sqrt(pow(dist_x,2)+pow(dist_y,2));
  

}



void girar(geometry_msgs::msg::Twist& vel,float radio){
      vel.linear.x = v;
      vel.angular.z = v/radio;
}

void stop(geometry_msgs::msg::Twist& vel){
      vel.linear.x = 0.0;
      vel.angular.z = 0.0;
}

void topic_callback(const nav_msgs::msg::Odometry& pos){
    if(first_read){
      position["ini_x"] = pos.pose.pose.position.x;
      position["ini_y"] = pos.pose.pose.position.y;
    
    }
    first_read = false;
    position["x"] = pos.pose.pose.position.x;
    position["y"] = pos.pose.pose.position.y;


}



int main (int argc, char* argv[]){
  rclcpp::init(argc,argv);
  auto node = rclcpp::Node::make_shared("Circle");
  auto publisher = node -> create_publisher<geometry_msgs::msg::Twist>("/cmd_vel",10);
  auto subscriber = node -> create_subscription<nav_msgs::msg::Odometry>("/odom",10,topic_callback);
  
  node -> declare_parameter("radio",1.0);
  float radio = node -> get_parameter("radio").get_parameter_value().get<float>();
  
  rclcpp::WallRate loop_rate(1000ms);
  
  
  geometry_msgs::msg::Twist vel;
  //&& dist(position["ini_x"],position["x"],position["ini_y"],position["y"]) > 0.1
  while (rclcpp::ok() && !circle_completed){
    girar(vel,radio);
    publisher -> publish(vel);
    rclcpp::spin_some(node);
    loop_rate.sleep();
    dist(position["ini_x"],position["x"],position["ini_y"],position["y"]);
  }
  for(int i = 0; i < 100; i++){
    stop(vel);
    publisher -> publish(vel);
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
  rclcpp::shutdown();
  return 0;
  

}
