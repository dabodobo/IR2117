#include <chrono>  
#include "rclcpp/rclcpp.hpp" 
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "cmath"
#include "map"


using namespace std::chrono_literals;
//std::shared_ptr<rclcpp::Publisher<std_msgs::msg::String>> publisher
std::map<std::string,float> position;
bool first_read = true;

void topic_callback(const nav_msgs::msg::Odometry msg){
	/*if (fisrt_read){
	    initial_position= {"x" : nav_msgs::msg::Odometry.pose.pose.position.x, "y" : nav_msgs::msg::Odometry.pose.pose.position.y }
	}
	*/
	position["x"] = msg.pose.pose.position.x;
	position["y"] = msg.pose.pose.position.y;
	 
	std::cout << "Position x: " << position["x"] << std:: endl;
	std::cout << "Position y: " << position["y"] << std:: endl;
	std::cout << "- - - - - - - - - - - " << std::endl << std::endl;
	
}


int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv); 
    auto node = rclcpp::Node::make_shared("node_position"); //creo el nodo odom
    auto subscription = node->create_subscription<nav_msgs::msg::Odometry>("odom", 10, topic_callback); //creo la suscripcion a odom u call_back
    //publisher = node->create_publisher<std_msgs::msg::String>("position", 10);
    
    rclcpp::spin(node); //importante 
    rclcpp::shutdown(); 
    return 0;
}

