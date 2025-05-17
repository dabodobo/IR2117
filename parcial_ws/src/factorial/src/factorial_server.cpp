#include <inttypes.h>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
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
// nos permite publicar el feedback

// creamos la conexión
rclcpp_action::GoalResponse handle_goal( const rclcpp_action::GoalUUID& uuid, std::shared_ptr<const Factorial::Goal> goal){
    if(goal > 0){
    RCLCPP_INFO(rclcpp::get_logger("server"),"Got goal request with order %d", goal -> numero);
    (void)uuid;
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE; // también puede ser REJECT
    }
    else{
        RCLCPP_INFO(rclcpp::get_logger("server"),"Invalid goal, it must be greater than 0 and you typed %d", goal -> numero);
        return rclcpp_action::GoalResponse::REJECT;
    }
}

//cancelar durante la ejecución
rclcpp_action::CancelResponse handle_cancel(const std::shared_ptr<GoalHandle> goal_handle){
    RCLCPP_INFO(rclcpp::get_logger("server"),"Got request to cancel goal");
    (void)goal_handle;
    return rclcpp_action::CancelResponse::ACCEPT;
}

//función de ejecución
void execute(const std::shared_ptr<GoalHandle> goal_handle){
    RCLCPP_INFO(rclcpp::get_logger("server"),"Executing goal . . . "); // mensaje de inicio de ejecución
    rclcpp::Rate loop_rate(1); // hacemos que se duerma un segundo

    const auto goal = goal_handle -> get_goal(); //Accede al goal real que envió el cliente a través del servidor
    auto feedback = std::make_shared<Factorial::Feedback>(); // es un puntero a un objeto feedback
    auto& secuencia_numeros = feedback -> secuencia_numeros; //vector
    auto& resultado_parcial = feedback -> resultado_parcial;
    resultado_parcial = 1;
    auto result = std::make_shared<Factorial::Result>();

    //ejecución del factorial
    for(int i = 1; (i < goal -> numero) && rclcpp::ok(); i++){
        if(goal_handle -> is_canceling()){
            result -> resultado = resultado_parcial;
            goal_handle -> canceled(result); // cancelamos el resultado
            RCLCPP_INFO(rclcpp::get_logger("server"),"Goal Canceled");
            return;
        }
        resultado_parcial = resultado_parcial *i; // operacion factorial
        secuencia_numeros.push_back(i);
        goal_handle -> publish_feedback(feedback); //nodo que publica el feedback
        RCLCPP_INFO(rclcpp::get_logger("server"),"Publish Feedback");
        loop_rate.sleep();

        if(rclcpp::ok()){
            result -> resultado = resultado_parcial;
            goal_handle -> succeed(result); // el resultado es correcto
            RCLCPP_INFO(rclcpp::get_logger("server"),"Goal Succeed");

        }

    }

}
void handle_accepted(const std::shared_ptr<GoalHandle> goal_handle){ // si se acepta el goal se ejecuta un hilo con el execute
    std::thread{execute,goal_handle}.detach();
}

int main(int argc, char ** argv){
    rclcpp::init(argc,argv);
    auto node = rclcpp::Node::make_shared("factorial_server");
    auto action_server = rclcpp_action::create_server<Factorial>(node,"factorial",handle_goal,handle_cancel,handle_accepted);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;

}