#include <inttypes.h>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp.action/rclcpp_action"
#include "factorial_interfaces/action/factorial.hpp"
#include <iostream>
#include <vector>
//
using Factorial = factorial_interfaces::action::Factorial;
using GoalHandle = rclcpp_action::ServerGoalHandle<Factorial>;
// 1) estado del goal: aceptado, ejecutandose, rechazado
// 2) feedback
// 3) finalizar con un resultado
// 4) Cancelar el goal

// creamos la conexión
rclcpp_action::GoalResponse handle_goal( const rclcpp_actiion::GoalUUID& uuid, std::shared_ptr<const Factorial::Goal> goal){
    if(goal > 0){
    RCLCPP_INFO(rclcpp::get_logger("server"),"Got goal request with order %d" goal -> numero);
    (void)uuid;
    return rclcpp_action::GoalRespondese::ACCEPT_AND_EXECUTE; // también puede ser REJECT
    }
    else{
        RCLCPP_INFO(rclcpp::get_logger("server"),"Invalid goal, it must be greater than 0 and you typed %d" goal -> numero);
        return rclcpp_action_GoalResponse::REJECT;
    }
}

//cancelar durante la ejecución
rclcpp_action::CancelResponse handle_cancle(const std::shared_ptr<GoalHandle> goal_handle){
    RCLCPP_INFO(rclcpp::get_logger("server"),"Got request to cancel goal");
    (void)goal_handle;
    return rclcpp_action::CancelResponse::ACCEPT;
}

//función de ejecución
void execute(const std::shared_ptr<GoalHandle> goal_handle);
    RCLCPP_INFO(get_logger("server"),"Executing goal . . . "); // mensaje de iniciio de ejecución
    rclcpp::Rate loop_rate(1) // hacemos la cola lo más pequeña posible

    const auto goal =


void handle_accepted(const std::shared_ptr<GoalHandle> goal_handle){

    std::thread{execute,goal_handle}.detach();
}