#include <inttypes.h>
#include <memory>
#include "action_tutorials_interfaces/action/fibonacci.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

using Fibonacci = action_tutorials_interfaces::action::Fibonacci;

using GoalHandleFibonacci = rclcpp_action::ServerGoalHandle<Fibonacci>;


// ACEPTACIÓN DEL GOAL -----

rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID & uuid, std::shared_ptr<const Fibonacci::Goal> goal){

  RCLCPP_INFO(rclcpp::get_logger("server"), "Got goal request with order %d", goal->order);
  (void)uuid;
  return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  
}

// -----------

// CANCELACIÓN DEL GOAL ----

rclcpp_action::CancelResponse handle_cancel(const std::shared_ptr<GoalHandleFibonacci> goal_handle){
  RCLCPP_INFO(rclcpp::get_logger("server"), "Got request to cancel goal");
  (void)goal_handle;
  return rclcpp_action::CancelResponse::ACCEPT;
}

// -----------

void execute(const std::shared_ptr<GoalHandleFibonacci> goal_handle){

  RCLCPP_INFO(rclcpp::get_logger("server"), "Executing goal");
  rclcpp::Rate loop_rate(1);
  
  const auto goal = goal_handle->get_goal();
  
  auto feedback = std::make_shared<Fibonacci::Feedback>();
  
  auto & sequence = feedback->partial_sequence;
  
  sequence.push_back(0);
  sequence.push_back(1);
  
  auto result = std::make_shared<Fibonacci::Result>();


void handle_accepted(const std::shared_ptr<GoalHandleFibonacci> goal_handle){
  std::thread{execute, goal_handle}.detach(); // se ejecuta el "execute" pasandole el goal_handle como argumento; todo esto en un hilo secundario separado del principal.
}

