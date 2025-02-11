#include <chrono>  
#include "rclcpp/rclcpp.hpp" 
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv); 
    auto node = rclcpp::Node::make_shared("publisher"); 
    auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10); 
    
    geometry_msgs::msg::Twist message;  //creo el mensaje cmd_vel

    rclcpp::WallRate loop_rate(10ms);
    
    int i = 0, n = 1000 // como voy a 0.1 m/s y hago un looprate de 10ms entonces para hacer un metro a una velocidad de 1 metro por segundo
    // necesito hacer 1000 iteraciones
    
    message.linear.x = 0.0;
    for(int j = 0; j < 4; j++){
    while (rclcpp::ok() && i < n) { 
    	i++;
    	message.linear.x = 10.0;
        publisher->publish(message);
        rclcpp::spin_some(node); 
        loop_rate.sleep(); 
    }
     message.linear.x = 0.0; // lo paro antes de girar
    publisher->publish(message);
    std::spin(node);
   
    int i = 0, n = 100 
    
    while(rclcpp::ok() && i < n;)
    	i++;
	message.angular.z = j % 2 == 0? -1.0; 1.0;
	publisher->publish(message);
	rclcpp::spin_some(node); 
        loop_rate.sleep();
    rclcpp::shutdown(); 
    }
    return 0;
}

