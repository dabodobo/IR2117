#include <chrono>  
#include "rclcpp/rclcpp.hpp" 
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv); 
    auto node = rclcpp::Node::make_shared("turtle_square"); 
    auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10); 
    
    geometry_msgs::msg::Twist message;  //creo el mensaje cmd_vel

    rclcpp::WallRate loop_rate(10ms);
    
 
    double velocidad_lineal = 0.1;
    double velocidad_angular = 0.1;
    message.linear.x = 0.0;
    for(int j = 0; j < 4; j++){
    
    int i = 0, n = 1 / (0.01 * velocidad_lineal);	
    	
    while (rclcpp::ok() && i < n) { 
    	i++;
    	message.linear.x = velocidad_lineal;
        publisher->publish(message);
        rclcpp::spin_some(node); 
        loop_rate.sleep(); 
    }
    message.linear.x = 0.0; // lo paro antes de girar
    publisher->publish(message);
    rclcpp::spin_some(node);
   
    i = 0; n = 1.57/(0.01*0.1); // n * 10 ms * v (rad/s) = rad = 3.14/2
    			// n = 1.57/(0.01*0.1)
    
    while(rclcpp::ok() && i < n){
    	i++;
	message.angular.z = velocidad_angular;
	publisher->publish(message);
	rclcpp::spin_some(node); 
        loop_rate.sleep();
        }
    message.angular.z = 0.0;
    publisher->publish(message);
    rclcpp::spin_some(node); 
    loop_rate.sleep();
    
    }
    
    
    
    rclcpp::shutdown(); 
    return 0;
}

