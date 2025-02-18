#include <chrono>  
#include "rclcpp/rclcpp.hpp" 
#include "geometry_msgs/msg/twist.hpp"
#include "cmath"

using namespace std::chrono_literals;

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv); 
    auto node = rclcpp::Node::make_shared("turtle_square"); 
    auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10); 
    
    geometry_msgs::msg::Twist message;  //creo el mensaje cmd_vel

    rclcpp::WallRate loop_rate(10ms);
    
     message.linear.x = 0.0;
 
   // estos son base, si o se declara
    node->declare_parameter("linear_speed",0.1);
    node->declare_parameter("angular_speed",0.1);
    node->declare_parameter("square_length",4.0);
    
    // si se los pasamos
    
    double linear_speed = node -> get_parameter("linear_speed").get_parameter_value().get<double>();
    double angular_speed = node -> get_parameter("angular_speed").get_parameter_value().get<double>();
    double square_length = node -> get_parameter("square_length").get_parameter_value().get<double>();
   
    
    for(int j = 0; j < 4; j++){
    
    int i = 0, n = square_length / (0.01 * linear_speed);	
    	
    while (rclcpp::ok() && i < n) { 
    	i++;
    	message.linear.x = linear_speed;
        publisher->publish(message);
        rclcpp::spin_some(node); 
        loop_rate.sleep(); 
    }
    std::cout << n << std::endl;
    message.linear.x = 0.0; // lo paro antes de girar
    publisher->publish(message);
    rclcpp::spin_some(node);
   
    i = 0; n = (M_PI/2)/(0.01*angular_speed); // n * 10 ms * v (rad/s) = rad = 3.14/2
    			// n = 1.57/(0.01*0.1)
    
    while(rclcpp::ok() && i < n){
    	i++;
	message.angular.z = angular_speed;
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

