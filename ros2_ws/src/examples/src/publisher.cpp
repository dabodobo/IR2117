#include <chrono>  
#include "rclcpp/rclcpp.hpp" //librería de ros
#include "std_msgs/msg/string.hpp" //describe los mensajes de tipo string
using namespace std::chrono_literals;
int main(int argc, char * argv[]){
	rclcpp::init(argc,argv); // imprescindible para inicializar (conectar) ros
	auto node = rclcpp::Node::make_shared("publisher"); //crea el nodo (puntero) y le pone el nombre siempre al principio del código.
	auto publisher = node->create_publisher<std_msgs::msg::String>("topic",10); // <> plantilla donde me marca el tipo del elemento, creación del topic.
	std_msgs::msg::String message; //variable de mensaje
	auto publish_count = 0;
	rclcpp::WallRate loop_rate(500ms);
	
	while(rclcpp::ok()){ //Se le pregunta a ros si está todo bien, es lo mismo que while true
		message.data = "Hello, world! " + std::to_string(publish_count++); //Creación del mensaje
		publisher->publish(message); // se envía el mensaje
		rclcpp::spin_some(node); //espera a hasta que se asegura que se ha enviado el topico
		loop_rate.sleep(); // tiempo de espera.
	
	}
	rclcpp::shutdown(); // siempre al final de terminar el main para cerrar el nodo
	return 0;



}
