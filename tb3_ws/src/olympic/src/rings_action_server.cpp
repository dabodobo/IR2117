#include <inttypes.h>
#include <memory>
#include "olympic_interfaces/action/rings.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "iostream"

#include <chrono>
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/srv/set_pen.hpp"
#include "turtlesim/srv/teleport_absolute.hpp"
#include <map>
#include <vector>

using Rings = olympic_interfaces::action::Rings;

using GoalHandleRings = rclcpp_action::ServerGoalHandle<Rings>;

using namespace std::chrono_literals;

// --- VARIABLES GLOBALES ----------
float lin_vel = 1.0; // velocidad lineal
bool apagar = true; // apagar el pen
//diccionario con los colores
std::map<std::string, std::vector<int>> pen = {
  {"apagar",{0,0,0} },
  {"rojo",{255,0,0}},
  {"azul",{0,0,255}},
  {"verde",{0,255,0}},
  {"amarillo",{255,255,0}},
  {"negro",{0,0,0}}

};
std::vector<std::pair<float,float>> pos = {{3.25,5.5},{5.5,5.5},{7.75,5.5},{4.35,4.5},{6.65,4.5}}; // orden de los tp's
std::vector<std::string> colores = {"azul","negro","rojo","amarillo","verde"}; // orden de los colores

geometry_msgs::msg::Twist vel; // twist que se publica para girar
int n = 0; // contador del circulo

// ---------------------------
// llamada al servicio pen

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


}

// llamada al servicio tp

void call_tp(float x, float y, rclcpp::Client<turtlesim::srv::TeleportAbsolute>::SharedPtr teleport_client){

  auto teleport = std::make_shared<turtlesim::srv::TeleportAbsolute::Request>();

  teleport->x = x;
  teleport->y = y;
  teleport->theta = 0;


  auto result_teleport = teleport_client->async_send_request(teleport);

}
// ACEPTACIÓN DEL GOAL -----

rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID & uuid, std::shared_ptr<const Rings::Goal> goal){

  RCLCPP_INFO(rclcpp::get_logger("server"), "Got goal request with radius %f", goal->radius);
  (void)uuid;
  return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  
}

// -----------

// CANCELACIÓN DEL GOAL ----

rclcpp_action::CancelResponse handle_cancel(const std::shared_ptr<GoalHandleRings> goal_handle){
  RCLCPP_INFO(rclcpp::get_logger("server"), "Got request to cancel goal");
  (void)goal_handle;
  return rclcpp_action::CancelResponse::ACCEPT;
}

// -----------


// EJECUCIÓN DE LA ACCIÓN ----
void execute(const std::shared_ptr<GoalHandleRings> goal_handle){

  RCLCPP_INFO(rclcpp::get_logger("server"), "Executing goal");
  rclcpp::Rate loop_rate2(1);

  // GOAL
  const auto goal = goal_handle->get_goal();
  const float radio = goal -> radius;

  // FEEDBACK
  auto feedback = std::make_shared<Rings::Feedback>(); //rings completed and angle
  int ring_number = feedback -> drawing_ring;
  float angle = feedback -> ring_angle;
  // RESULT

  auto result = std::make_shared<Rings::Result>();
  int rings_completed = result -> rings_completed;
 
  rclcpp::WallRate loop_rate(700ms);
  auto node = rclcpp::Node::make_shared("rings"); //Creación del nodo

  auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10); // Creación del publisher

  // Cliente color -----------------------

  rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr client = node -> create_client<turtlesim::srv::SetPen>("turtle1/set_pen");

  // Cliente teleporting

  rclcpp::Client<turtlesim::srv::TeleportAbsolute>::SharedPtr teleport_client = node -> create_client<turtlesim::srv::TeleportAbsolute>("turtle1/teleport_absolute");

  // ------------------

  for(int r = 0; r < 5; r++){

    // en caso de cancelación ...
    if ( goal_handle->is_canceling() ) {

      result->rings_completed = rings_completed;
      goal_handle->canceled(result);

      RCLCPP_INFO(rclcpp::get_logger("server"), "Goal Canceled");

      return;

    }

    apagar = true; // apago antes de hacer tp para que el pen no dibuje donde no quiero
    call_pen("apagar",client);
    call_tp(pos[r].first, pos[r].second, teleport_client); // hago tp
    apagar = false; // enciendo el pen
    call_pen(colores[r],client); // selecciono el color
    std::cout << "TP: " << pos[r].first << " " << pos[r].second << std::endl; // seguidor de las posiciones

    while (rclcpp::ok() && n <= 10) {

      vel.linear.x = lin_vel;
      vel.angular.z = lin_vel / radio; // w = v /r

      publisher->publish(vel);
      rclcpp::spin_some(node);
      loop_rate2.sleep();
      n++;


    }
    n = 0;

  }

}





void handle_accepted(const std::shared_ptr<GoalHandleRings> goal_handle){
  std::thread{execute, goal_handle}.detach(); // se ejecuta el "execute" pasandole el goal_handle como argumento; todo esto en un hilo secundario separado del principal.
}


// MAIN ----------

int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("action_server");
  auto action_server = rclcpp_action::create_server<Rings>(
      node,
      "rings",
      handle_goal,
      handle_cancel,
      handle_accepted
  );
  
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}


