#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <iostream>

void topic_callback(const std_msgs::msg::String::SharedPtr msg){ //el call_back
	std::cout<< msg->data << std::endl;
}

int main(int argc, char * argv[]){
	rclcpp::init(argc,argv);
	auto node = rclcpp::Node::make_shared("subscriber");
	auto subscription = node->create_subscription<std_msgs::msg::String>("topic",10,topic_callback); //call_back: realiza la acción cuando llega el msg. Se el da el nombre del topico(el mismo que el suscriptor) y la funcion a la que se llamará cuando se reciba el msg
	rclcpp::spin(node); //bucle interno
	rclcpp::shutdown();//apagar
	return 0;


}
