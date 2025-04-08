#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"
#include <memory>
using example_interfaces::srv::AddTwoInts;

void add(
  std::shared_ptr<AddTwoInts::Request>  request, // creo el puntero a pregunta
  std::shared_ptr<AddTwoInts::Response> response) // creo el puntero a respuesta
{
  response->sum = request->a + request->b; // la respuesta es la suma de los 2 parametros de la pregunta

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), //información por pantalla
    "Incoming request\na: %ld" " b: %ld",
     request->a, request->b);
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), 
    "sending back response: [%ld]", 
     (long int)response->sum);
}

// -- EMPIEZA EL MAIN --

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv); //iniciamos el nodo

  std::shared_ptr<rclcpp::Node> node = 
    rclcpp::Node::make_shared("add_two_ints_server"); // creamos el nodo

  rclcpp::Service<AddTwoInts>::SharedPtr service =
	 node->create_service<AddTwoInts>("add_two_ints", &add); //creamos el nodo de servicio

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), //información por pantalla
    "Ready to add two ints.");

  rclcpp::spin(node);
  rclcpp::shutdown();
}

