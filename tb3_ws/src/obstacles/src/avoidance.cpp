#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "example_interfaces/msg/bool.hpp"

using namespace std::chrono_literals;


bool front_obstacle= false, left_obstacle= false, right_obstacle = false;

void callback_front(const example_interfaces::msg::Bool msg){
	front_obstacle = msg.data;

}

void callback_left(const example_interfaces::msg::Bool msg){
	left_obstacle = msg.data;

}

void callback_right(const example_interfaces::msg::Bool msg){
	right_obstacle = msg.data;

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

void random_choice(geometry_msgs::msg::Twist& vel){
	double random = std::rand();
	if(random >= 0.5){
		girar_izq(vel);
	}
	else{
	
		girar_der(vel);
	}
}



int main(int argc, char* argv[]){

	rclcpp::init(argc,argv);
	auto node = rclcpp::Node::make_shared("avoidance");
	auto publisher = node -> create_publisher<geometry_msgs::msg::Twist>("cmd_vel",10);
	
	auto subs_front = node -> create_subscription<example_interfaces::msg::Bool("/front/obstacle)",10,callback_front);
	auto subs_left = node -> create_subscription<example_interfaces::msg::Bool("/left/obstacle)",10,callback_front);
	auto subs_right = node -> create_subscription<example_interfaces::msg::Bool("/right/obstacle)",10,callback_front);
	geometry_msgs::msg::Twist vel;
	rclcpp::WallRate loop_rate(50ms);
	
	while(rclcpp::ok()){
		if(!front_obstacle){
			avanzar(vel);
		}
		else if(front_obstacle && right_obstacle){
			girar_izq(vel);
		}
		else if(front_obstacle && left_obstacle){
			girar_der(vel);
		}
		else if(front_obstacle){
			random_choice(vel);
		}
		
		publisher->publish(vel);
		rclcpp::spin_some(node);
		loop_rate.sleep();
	
	}
	rclcpp::shutdown();
	return 0;




}
