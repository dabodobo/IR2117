#include <inttypes.h>
#include <memory>
#include "action_tutorials_interfaces/action/fibonacci.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"


using Fibonacci = action_tutorials_interfaces::action::Fibonacci; //action

using GoalHandleFibonacci = rclcpp_action::ClientGoalHandle<Fibonacci>; //goal

using namespace std::chrono_literals;

rclcpp::Node::SharedPtr g_node = nullptr; // pointer al ros node


// Se ejecutará si se ha aceptado el goal ---
void feedback_callback(GoalHandleFibonacci::SharedPtr,const std::shared_ptr<const Fibonacci::Feedback> feedback){
  RCLCPP_INFO(g_node->get_logger(),"Next number in sequence received: %" PRId32,feedback->partial_sequence.back());
}


// --------

int main(int argc, char ** argv){

  rclcpp::init(argc, argv);
  g_node = rclcpp::Node::make_shared("action_client");
  
  auto action_client = rclcpp_action::create_client<Fibonacci>(g_node, "fibonacci"); // fibonacci es el nombre del servidor de acciones y g_node es el puntero al nodo cliente.

  // esperando a encontrar el service disponible ...
  
  if (!action_client->wait_for_action_server(20s)) {
  
    RCLCPP_ERROR(g_node->get_logger(), "Action server not available after waiting");
    
    return 1;
  }
  
  // Declaración del goal
  auto goal_msg = Fibonacci::Goal();
  goal_msg.order = 10;


  // Pregunta al server si puede realizar la acción --
  RCLCPP_INFO(g_node->get_logger(), "Sending goal");
  
  auto send_goal_options = rclcpp_action::Client<Fibonacci>::SendGoalOptions();
  
  send_goal_options.feedback_callback = feedback_callback;
  
  auto goal_handle_future = action_client->async_send_goal(goal_msg, send_goal_options);
	
  //espera que ha sido aceptado o rechazado el goal -- 
  auto return_code = rclcpp::spin_until_future_complete(g_node,goal_handle_future);
  
  // Sale encaso de fallo o ha sido rechazado por el server
  
   if (return_code != rclcpp::FutureReturnCode::SUCCESS){
    RCLCPP_ERROR(g_node->get_logger(), "send goal call failed :(");
    return 1;
  }

  GoalHandleFibonacci::SharedPtr goal_handle = goal_handle_future.get();
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
  
  GoalHandleFibonacci::WrappedResult wrapped_result = result_future.get();

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
  
  for (auto number : wrapped_result.result->sequence) {
  
    RCLCPP_INFO(g_node->get_logger(), "%" PRId32, number);
    
  }

  action_client.reset();
  g_node.reset();
  rclcpp::shutdown();
  return 0;
}

  






