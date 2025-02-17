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
    
 
    
    message.linear.x = 0.0;
    /*
    for(int j = 0; j < 4; j++){
    
    	int i = 0, n = 1 / (0.01 * 0.1);	
    	
    while (rclcpp::ok() && i < n) { 
    	i++;
    	message.linear.x = 2.0;
        publisher->publish(message);
        rclcpp::spin_some(node); 
        loop_rate.sleep(); 
    }
    message.linear.x = 0.0; // lo paro antes de girar
    publisher->publish(message);
    rclcpp::spin_some(node);
   
    i = 0; n = 100; 
    
    while(rclcpp::ok() && i < n){
    	i++;
	message.angular.z = 1.57; //1 rad/seg
	publisher->publish(message);
	rclcpp::spin_some(node); 
        loop_rate.sleep();
        }
    message.angular.z = 0.0;
    publisher->publish(message);
    rclcpp::spin_some(node); 
    loop_rate.sleep();
    
    }
    */ // linear
    
    int i = 0, n = 1 / (0.01 * 0.1);	
    	
    while (rclcpp::ok() && i < n) { 
    	i++;
    	message.linear.x = 0.1;
        publisher->publish(message);
        rclcpp::spin_some(node); 
        loop_rate.sleep(); 
    }
    
    
    
    
    rclcpp::shutdown(); 
    return 0;
}

