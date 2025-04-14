#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "example_interfaces/msg/bool.hpp"

using namespace std::chrono_literals;


bool front_person= false, left_person= false, right_person = false;

void callback_front(const example_interfaces::msg::Bool msg){
	front_person = msg.data;

}

void callback_left(const example_interfaces::msg::Bool msg){
	left_person = msg.data;

}

void callback_right(const example_interfaces::msg::Bool msg){
	right_person = msg.data;

}

void avanzar (geometry_msgs::msg::Twist& vel){
	vel.linear.x = 0.1;
	vel.angular.z = 0.0;
	//std::cout << "AVANZ0 !! " << std::endl;
}

void girar_izq(geometry_msgs::msg::Twist& vel){
	vel.linear.x = 0.0;
	vel.angular.z = 0.1;
	//std::cout << "IZQUIERDA !! " << std::endl;
}

void girar_der(geometry_msgs::msg::Twist& vel){
	vel.linear.x = 0.0;
	vel.angular.z = -0.1;
	//std::cout << "DERECHA !! " << std::endl;
}

void stop(geometry_msgs::msg::Twist& vel){
	vel.linear.x = 0.0;
	vel.angular.z = 0.0;
}





int main(int argc, char* argv[]){

	rclcpp::init(argc,argv);
	
	auto node = rclcpp::Node::make_shared("follower");
	auto publisher = node -> create_publisher<geometry_msgs::msg::Twist>("/cmd_vel",10);
	
	auto subs_front = node -> create_subscription<example_interfaces::msg::Bool>("/front/person",10,callback_front);
	auto subs_left = node -> create_subscription<example_interfaces::msg::Bool>("/left/person",10,callback_left);
	auto subs_right = node -> create_subscription<example_interfaces::msg::Bool>("/right/person",10,callback_right);
	geometry_msgs::msg::Twist vel;
	rclcpp::WallRate loop_rate(50ms);
	
	while(rclcpp::ok()){
		if(front_person){
			avanzar(vel);
		}
		else if(right_person){
			girar_der(vel);
		}
		else if(left_person){
			girar_izq(vel);
		}
		else{
			stop(vel);
		}
		
		publisher->publish(vel);
		rclcpp::spin_some(node);
		loop_rate.sleep();
	
	}
	rclcpp::shutdown();
	return 0;




}
