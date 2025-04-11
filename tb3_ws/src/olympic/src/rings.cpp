#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/srv/set_pen.hpp"
#include "turtlesim/srv/teleport_absolute.hpp"
#include <map>
#include <vector>
#include <iostream>

using namespace std::chrono_literals;


// --- VARIABLES GLOBALES ----------
float lin_vel = 1.0;
bool apagar = true;
std::map<std::string, std::vector<int>> pen = {
  {"apagar",{0,0,0} },
  {"rojo",{255,0,0}},
  {"azul",{0,0,255}},
  {"verde",{0,255,0}},
  {"amarillo",{255,255,0}},
  {"negro",{0,0,0}}
  
};
std::vector<std::pair<float,float>> pos = {{3.25,5.5},{5.5,5.5},{7.75,5.5},{4.35,4.5},{6.65,4.5}};
std::vector<std::string> colores = {"azul","negro","rojo","amarillo","verde"};

geometry_msgs::msg::Twist vel;
int n = 0;

// ---------------------------

void call_pen(std::string color, rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr client){

  auto color_pen = std::make_shared<turtlesim::srv::SetPen::Request>(); // ponemos las variables
 
  color_pen->r = pen[color][0];
  color_pen->g = pen[color][1];
  color_pen->b = pen[color][2];
  color_pen->width = 3;
  if(apagar){
  color_pen->off = 1;
  }
  else{
  color_pen->off = 0;
  }
  
  
  auto result = client->async_send_request(color_pen);
  apagar = false;
  /*
  if (rclcpp::spin_until_future_complete(node,result) == rclcpp::FutureReturnCode::SUCCESS)
  {
    RCLCPP_INFO(rclcpp::get_logger("color"), 
     "Color cambiado con éxito");
  } else {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), 
     "Ha habido un error al llamar al servicio de color");
  }
  
*/
}

void call_tp(float x, float y, rclcpp::Client<turtlesim::srv::TeleportAbsolute>::SharedPtr teleport_client){

  auto teleport = std::make_shared<turtlesim::srv::TeleportAbsolute::Request>();
 
  teleport->x = x;
  teleport->y = y;
  teleport->theta = 0;
  
 
  auto result_teleport = teleport_client->async_send_request(teleport);
/*
  if (rclcpp::spin_until_future_complete(node,result_teleport) == rclcpp::FutureReturnCode::SUCCESS)
  {
    RCLCPP_INFO(rclcpp::get_logger("teleport"), 
     "Teleportado con éxito");
  } else {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), 
     "Ha habido un error al llamar al servicio de teleport");
  }
*/

}



int main(int argc, char * argv[])
{
 rclcpp::init(argc, argv);
 rclcpp::WallRate loop_rate(700ms);
 
 /* --- ¿Numero de argumentos correctos? ---
 
 if(argc != 6 ){
    RCLCPP_INFO(rclcpp::get_logger("args"), "uso: rings int_rojo int_verde int_azul x y"); // es el nombre del mensaje
    return 1;
 }
 
 // -----------------------------------------
 */
 
 auto node = rclcpp::Node::make_shared("rings");
 
 auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);
 
 
 node -> declare_parameter("radius",1.0); // necesitamos declarar previamente el parametro en el nodo antes de obtenerlo en la linea siguiente
 float radio =  node -> get_parameter("radius").get_parameter_value().get<double>();
 
 // Cliente color -----------------------
 
 rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr client = node -> create_client<turtlesim::srv::SetPen>("turtle1/set_pen"); 
  
  // Cliente teleporting
  
  rclcpp::Client<turtlesim::srv::TeleportAbsolute>::SharedPtr teleport_client = node -> create_client<turtlesim::srv::TeleportAbsolute>("turtle1/teleport_absolute");
  
  // ------------------
  
  for(int r = 0; r < 5; r++){
    apagar = true;
    call_pen("apagar",client);
    call_tp(pos[r].first, pos[r].second, teleport_client);
    apagar = false;
    call_pen(colores[r],client);
    std::cout << "TP: " << pos[r].first << " " << pos[r].second << std::endl;
    
    while (rclcpp::ok() && n <= 10) {
   
     vel.linear.x = lin_vel;
     vel.angular.z = lin_vel / radio; // w = v /r
  
     publisher->publish(vel);
     rclcpp::spin_some(node);
     loop_rate.sleep();
     n++;
     
     
    }
    n = 0;
  
  }
  
 rclcpp::shutdown();
 return 0;
 
}

