#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/srv/set_pen.hpp"


float lin_vel = 1.0;


int main(int argc, char * argv[])
{
 rclcpp::init(argc, argv);
 
 // --- ¿Numero de argumentos correctos? ---
 if(argc != 4 ){
    RCLCPP_INFO(rclcpp::get_logger("args"), "uso: rings int_rojo int_verde int_azul"); // es el nombre del mensaje
    return 1;
 }
 
 
 auto node = rclcpp::Node::make_shared("rings");
 
 auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);
 
 
 node -> declare_parameter("radius",1.0); // necesitamos declarar previamente el parametro en el nodo antes de obtenerlo en la linea siguiente
 float radio =  node -> get_parameter("radius").get_parameter_value().get<double>();
 
 // Cliente color -----------------------
 
 rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr client = node -> create_client<turtlesim::srv::SetPen>("turtle1/set_pen"); 
 auto request = std::make_shared<turtlesim::srv::SetPen::Request>();
 
  request->r = atoll(argv[1]); // atoll cambia de string a int
  request->g = atoll(argv[2]); // atoll cambia de string a int
  request->b = atoll(argv[3]); // atoll cambia de string a int
  request->width = 3;
  request->off = 0;
 
  auto result = client->async_send_request(request);
  
  if (rclcpp::spin_until_future_complete(node,result) == rclcpp::FutureReturnCode::SUCCESS)
  {
    RCLCPP_INFO(rclcpp::get_logger("color"), 
     "Color cambiado con éxito");
  } else {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), 
     "Ha habido un error al llamar al servicio de color");
  }
  
  // --------------------------------------
  
 geometry_msgs::msg::Twist vel;
 while (rclcpp::ok()) {
 
   vel.linear.x = lin_vel;
   vel.angular.z = lin_vel / radio; // w = v /r
   
   publisher->publish(vel);
   rclcpp::spin_some(node);
   
  }
  
 rclcpp::shutdown();
 return 0;
 
}

