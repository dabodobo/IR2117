#include <chrono>  
#include "rclcpp/rclcpp.hpp" 
#include "geometry_msgs/msg/twist.hpp"
#include "cmath"

using namespace std::chrono_literals;

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv); 
    auto node = rclcpp::Node::make_shared("polygon"); 
    auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10); 
    
    geometry_msgs::msg::Twist message;  //creo el mensaje cmd_vel

    rclcpp::WallRate loop_rate(10ms);
    
    message.linear.x = 0.0; // empieza en stop
 
 
    node->declare_parameter("linear_speed",0.8); //double
    node->declare_parameter("angular_speed",0.5); //double
    node->declare_parameter("segment_size",0.5); //double
    node->declare_parameter("number_segments",4); //int
    
    
    
    double linear_speed = node -> get_parameter("linear_speed").get_parameter_value().get<double>();
    double angular_speed = node -> get_parameter("angular_speed").get_parameter_value().get<double>();
    double segment_size = node -> get_parameter("segment_size").get_parameter_value().get<double>();
    int number_segments = node -> get_parameter("number_segments").get_parameter_value().get<int>();
   
    
    for(int j = 0; j < number_segments; j++){
    
	    int i = 0, n = segment_size / (0.01 * linear_speed);	 
	    	
	    while (rclcpp::ok() && i < n) {   // este while avanza
	    	i++;
	    	message.linear.x = linear_speed;
		publisher->publish(message);
		rclcpp::spin_some(node); 
		loop_rate.sleep(); 
	    }
	    
	    // lo paro antes de girar
	    message.linear.x = 0.0; 
	    publisher->publish(message);
	    rclcpp::spin_some(node);
	    loop_rate.sleep();
	   
	    i = 0; n = ((2*M_PI)/number_segments)/(0.01*angular_speed); // n * 10 ms * v (rad/s) = rad = 3.14*2 / N
	    			
	    
	    while(rclcpp::ok() && i < n){  // este while gira
	    	i++;
		message.angular.z = angular_speed;
		publisher->publish(message);
		rclcpp::spin_some(node); 
		loop_rate.sleep();
		}
	
	// lo paro antes de avanzar	
	    message.angular.z = 0.0;
	    publisher->publish(message);
	    rclcpp::spin_some(node); 
	    loop_rate.sleep();
    
    }
    
    
    
    rclcpp::shutdown(); 
    return 0;
}

