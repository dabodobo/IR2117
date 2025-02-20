#include <chrono>  
#include "rclcpp/rclcpp.hpp" 
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "cmath"
#include "map"


float quat(float x, float y, float z, float w){
	float siny_cosp = 2 *(w*z + x*y);
	float cosy_cosp = 1 - 2*(y*y +z*z);
	double yaw = std::atan2(siny_cosp, cosy_cosp); 
	return yaw;
}

float dist(float base_x,float base_y, float x, float y){
	float dist_x = x - base_x;
	float dist_y = y - base_y;
	return sqrt(dist_x*dist_x + dist_y*dist_y);
}
float angle_dist(float base_angle, float current){
	float angle_dist(float base_angle, float current) {
	    float dist = current - base_angle;
	    
	    // Normalizar la diferencia al rango [-π, π]
	    if (dist > M_PI) {
		dist -= 2 * M_PI;  // Si es mayor que π, restamos 2π
	    } else if (dist < -M_PI) {
		dist += 2 * M_PI;  // Si es menor que -π, sumamos 2π
	    }

	    return dist;
	}
}

using namespace std::chrono_literals;
//std::shared_ptr<rclcpp::Publisher<std_msgs::msg::String>> publisher
std::map<std::string,float> position;
bool first_read = true;

void topic_callback(const nav_msgs::msg::Odometry msg){
	float angle_x = msg.pose.pose.orientation.x;
	float angle_y = msg.pose.pose.orientation.y;
	float angle_z = msg.pose.pose.orientation.z;
	float angle_w = msg.pose.pose.orientation.w;
	if (first_read){
	    position["x"] = msg.pose.pose.position.x;
	    position["y"] = msg.pose.pose.position.y;
	    position["angle"] = quat(angle_x,angle_y,angle_z,angle_w);
	}
	first_read = false;
	float x =  msg.pose.pose.position.x;
	float y = msg.pose.pose.position.y;
	float angle = quat(angle_x,angle_y,angle_z,angle_w);
	 
	std::cout << "Position x: " << x << std:: endl;
	std::cout << "Position y: " << y << std:: endl;
	std::cout << "Angle : " << angle << std::endl;
	std::cout << "Distance : " << dist(position["x"],position["y"],x,y) << std::endl;
	std::cout << "Dif angle: " << angle_dist(position["angle"],angle) << std::endl;
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

