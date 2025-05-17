#include <inttypes.h>
#include <memory>
#include "olympic_interfaces/action/rings.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include <iostream>

using Rings = olympic_interfaces::action::Rings; //action

using GoalHandleRings = rclcpp_action::ClientGoalHandle<Rings>; //goal

using namespace std::chrono_literals;

rclcpp::Node::SharedPtr g_node = nullptr; // pointer al ros node




void load_bar(int valor, int max, std::string texto){
  int p =  100 *(valor+1) / max;
  std::cout  << texto << ": " << p << "%  [ |";
  for(int i = 0; i != valor; i++){
    std::cout << "|" ;
  }
  for(int i = valor; i < (max-valor); i++){
    std::cout << " " ;

  }
  std::cout << " ]" << std::endl << std::endl;

}
// Se ejecutará si se ha aceptado el goal ---
void feedback_callback(GoalHandleRings::SharedPtr,const std::shared_ptr<const Rings::Feedback> feedback){
  RCLCPP_INFO(g_node->get_logger(),"Current ring: %" PRId32,feedback->drawing_ring);
  RCLCPP_INFO(g_node->get_logger(),"Current angle: %f" ,feedback->ring_angle);
  load_bar(feedback->drawing_ring -1 ,5,"Ring");
  //load_bar(feedback->ring_angle,10,"Angle");


}




// --------

int main(int argc, char ** argv){

  rclcpp::init(argc, argv);
  g_node = rclcpp::Node::make_shared("action_client");
  
  auto action_client = rclcpp_action::create_client<Rings>(g_node, "rings"); // ring es el nombre del servidor de acciones y g_node es el puntero al nodo cliente.

  // esperando a encontrar el service disponible ...
  
  if (!action_client->wait_for_action_server(20s)) {
  
    RCLCPP_ERROR(g_node->get_logger(), "Action server not available after waiting");
    
    return 1;
  }
  
  // Declaración del goal
  auto goal_msg = Rings::Goal();
  goal_msg.radius = 1.0;


  // Pregunta al server si puede realizar la acción --
  RCLCPP_INFO(g_node->get_logger(), "Sending goal");
  
  auto send_goal_options = rclcpp_action::Client<Rings>::SendGoalOptions();
  
  send_goal_options.feedback_callback = feedback_callback;
  
  auto goal_handle_future = action_client->async_send_goal(goal_msg, send_goal_options);
	
  //espera que ha sido aceptado o rechazado el goal -- 
  auto return_code = rclcpp::spin_until_future_complete(g_node,goal_handle_future);
  
  // Sale encaso de fallo o ha sido rechazado por el server
  
   if (return_code != rclcpp::FutureReturnCode::SUCCESS){
    RCLCPP_ERROR(g_node->get_logger(), "send goal call failed :(");
    return 1;
  }

  GoalHandleRings::SharedPtr goal_handle = goal_handle_future.get();
  if (!goal_handle) {
    RCLCPP_ERROR(g_node->get_logger(), "Goal was rejected by server");
    return 1;
  }
  
  // -------------- Ahora epera a que el servidor realice la acción.
  
  auto result_future = action_client->async_get_result(goal_handle);

  RCLCPP_INFO(g_node->get_logger(), "Waiting for result");

  return_code = rclcpp::spin_until_future_complete(g_node, 
    result_future);

  if (return_code != rclcpp::FutureReturnCode::SUCCESS){
  
    RCLCPP_ERROR(g_node->get_logger(), "get result call failed :(");
    return 1;
    
  }
  
  GoalHandleRings::WrappedResult wrapped_result = result_future.get();

  switch (wrapped_result.code) {
  
    case rclcpp_action::ResultCode::SUCCEEDED:
      break;
      
    case rclcpp_action::ResultCode::ABORTED:
      RCLCPP_ERROR(g_node->get_logger(), "Goal was aborted");
      return 1;
      
    case rclcpp_action::ResultCode::CANCELED:
      RCLCPP_ERROR(g_node->get_logger(), "Goal was canceled");
      return 1;
      
    default:
      RCLCPP_ERROR(g_node->get_logger(), "Unknown result code");
      return 1;
      
  }
  RCLCPP_INFO(g_node->get_logger(), "result received");
 /*
  for (auto factorial : wrapped_result.result->sequence) {
  
    RCLCPP_INFO(g_node->get_logger(), "%" PRId32, factorial);
    
  }
  */

  action_client.reset();
  g_node.reset();
  rclcpp::shutdown();
  return 0;
}

  
