#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"


using namespace std::chrono_literals;

int main(int argc,char * argv[]){

	rclcpp::init(argc,argv);
	
	auto node = rclcpp::Node::make_shared("square");
	auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("cmd_vel",10);
	geometry_msgs::msg::Twist msg;
	rclcpp::WallRate loop_rate(10ms);
	
	for(int i = 0; i < 4; i++){
		int iter = 1000;
		int n = 0;
		while( n < iter){
			msg.linear.x = 1.0;
			publisher -> publish(msg);
			n++;
			rclcpp::spin_some(node);
			loop_rate.sleep();
		
		}
		msg.linear.x = 0.0;
			publisher -> publish(msg);
		rclcpp::spin_some(node);
		
		iter = 1570;
		n = 0;
		while( n < iter){
			msg.angular.z = 1.0;
			publisher -> publish(msg);
			n++;
			rclcpp::spin_some(node);
			loop_rate.sleep();
		
		}
		msg.angular.z = 0.0;
		publisher -> publish(msg);
		rclcpp::spin_some(node);
	        loop_rate.sleep();
		
	}
	rclcpp::shutdown();
	return 0;
}
