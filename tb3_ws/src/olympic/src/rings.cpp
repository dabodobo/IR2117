#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/srv/set_pen.hpp"
#include "turtlesim/srv/teleport_absolute.hpp"

float lin_vel = 1.0;


int main(int argc, char * argv[])
{
 rclcpp::init(argc, argv);
 
 // --- ¿Numero de argumentos correctos? ---
 
 if(argc != 6 ){
    RCLCPP_INFO(rclcpp::get_logger("args"), "uso: rings int_rojo int_verde int_azul x y"); // es el nombre del mensaje
    return 1;
 }
 
 // -----------------------------------------
 
 auto node = rclcpp::Node::make_shared("rings");
 
 auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);
 
 
 node -> declare_parameter("radius",1.0); // necesitamos declarar previamente el parametro en el nodo antes de obtenerlo en la linea siguiente
 float radio =  node -> get_parameter("radius").get_parameter_value().get<double>();
 
 // Cliente color -----------------------
 
 rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr client = node -> create_client<turtlesim::srv::SetPen>("turtle1/set_pen"); 
 auto color_pen = std::make_shared<turtlesim::srv::SetPen::Request>();
 
  color_pen->r = atoll(argv[1]); // atoll cambia de string a int
  color_pen->g = atoll(argv[2]); // atoll cambia de string a int
  color_pen->b = atoll(argv[3]); // atoll cambia de string a int
  color_pen->width = 3;
  color_pen->off = 0;
 
  auto result = client->async_send_request(color_pen);
  
  if (rclcpp::spin_until_future_complete(node,result) == rclcpp::FutureReturnCode::SUCCESS)
  {
    RCLCPP_INFO(rclcpp::get_logger("color"), 
     "Color cambiado con éxito");
  } else {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), 
     "Ha habido un error al llamar al servicio de color");
  }
  
  // --------------------------------------
  
  // Cliente teleporting
  
  rclcpp::Client<turtlesim::srv::TeleportAbsolute>::SharedPtr teleport_client = node -> create_client<turtlesim::srv::TeleportAbsolute>("turtle1/teleport_absolute");
  auto teleport = std::make_shared<turtlesim::srv::TeleportAbsolute::Request>();
 
  teleport->x = atoll(argv[4]); // atoll cambia de string a int
  teleport->y = atoll(argv[5]); // atoll cambia de string a int
  teleport->theta = 0;
  
 
  auto result_teleport = teleport_client->async_send_request(teleport);
  
  if (rclcpp::spin_until_future_complete(node,result_teleport) == rclcpp::FutureReturnCode::SUCCESS)
  {
    RCLCPP_INFO(rclcpp::get_logger("teleport"), 
     "Teleportado con éxito");
  } else {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), 
     "Ha habido un error al llamar al servicio de teleport");
  }
  //Esto se puede optimizar con una función.
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

