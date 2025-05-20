#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "example_interfaces/msg/bool.hpp"
#include <cmath>
#include <iostream>
#include <vector>
#include <chrono>
using namespace std::chrono_literals;
double angle;

std::vector<double> medidas = {M_PI/4,M_PI/2,2*M_PI/3, M_PI,-M_PI/4,-M_PI/2,-2*M_PI/3, -M_PI};

bool front =false,left = false,right=false;

void front_callback(const example_interfaces::msg::Bool::SharedPtr msg){
    front = msg -> data;
}
void left_callback(const example_interfaces::msg::Bool::SharedPtr msg){
    left= msg -> data;
}
void right_callback(const example_interfaces::msg::Bool::SharedPtr msg){
    right = msg -> data;
}

float quat(float x, float y, float z, float w){
    float siny_cosp = 2 *(w*z + x*y);
    float cosy_cosp = 1 - 2*(y*y +z*z);
    double yaw = std::atan2(siny_cosp, cosy_cosp);
    return yaw;
}
float angle_dist(float base_angle, float current){

    float dist = current - base_angle;

    // Normalizar la diferencia al rango [-π, π]
    if (dist > M_PI) {
        dist -= 2 * M_PI;  // Si es mayor que π, restamos 2π
    } else if (dist < -M_PI) {
        dist += 2 * M_PI;  // Si es menor que -π, sumamos 2π
    }

    return dist;

}
void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg)
{
    angle = quat(msg->pose.pose.orientation.x,
                 msg->pose.pose.orientation.y,
                 msg->pose.pose.orientation.z,
                 msg->pose.pose.orientation.w);
}

void turn_left(geometry_msgs::msg::Twist& vel){
    vel.angular.z = 0.1;
}
void turn_right(geometry_msgs::msg::Twist& vel){
    vel.angular.z = -0.1;
}
void stop(geometry_msgs::msg::Twist& vel){
    vel.angular.z = 0.0;
}
void turbo(const double distancia,geometry_msgs::msg::Twist& vel){
    double aumento_velocidad;
    if (distancia > 2.8){
        aumento_velocidad =1.0;
    }
    else if(distancia > 2){
        aumento_velocidad = 0.5;
    }
    else if(distancia > 1.4){
        aumento_velocidad = 0.3;
    }
    else{
        aumento_velocidad = 0.2;
    }
    vel.angular.z = aumento_velocidad;
}
int main(int argc, char ** argv){
    rclcpp::init(argc,argv);
    auto node = rclcpp::Node::make_shared("rolling");
    auto publisher = node -> create_publisher<geometry_msgs::msg::Twist>("/cmd_vel",10);
    auto subscriber = node -> create_subscription<nav_msgs::msg::Odometry>("/odom",10,odom_callback);
    auto sub_front = node -> create_subscription<example_interfaces::msg::Bool>("/front/obstacle",10,front_callback);
    auto sub_left = node -> create_subscription<example_interfaces::msg::Bool>("/left/obstacle",10,left_callback);
    auto sub_right = node -> create_subscription<example_interfaces::msg::Bool>("/right/obstacle",10,right_callback);
    geometry_msgs::msg::Twist vel;
    rclcpp::WallRate loop_rate(100ms);

    double goal;
    double distancia;
    bool girar = false;
    while(rclcpp::ok()){

        girar = front || right || left;

        if (girar) {
            if(front){
                goal = 0;
            }
            else if(right){
                goal = -M_PI/2;
            }
            else if(left){
                goal = M_PI/2;
            }

            distancia = angle_dist(angle, goal);
            std::cout << "Voy a girar hacia " << goal << " (distancia: " << distancia << " rad)" << std::endl;

            while(rclcpp::ok() && std::abs(distancia) > 0.2){
                rclcpp::spin_some(node);
                distancia = angle_dist(angle, goal);

                if(goal < 0){
                    turn_right(vel);
                }
                else{
                    turn_left(vel);
                }


                publisher->publish(vel);
                rclcpp::spin_some(node);
                loop_rate.sleep();
            }

            stop(vel);
            publisher->publish(vel);
            rclcpp::spin_some(node);
            loop_rate.sleep();
            std::cout << "Parado" << std::endl;
        }
        stop(vel);
        publisher->publish(vel);
        rclcpp::spin_some(node);
        loop_rate.sleep();
        std::cout << "Parado" << std::endl;
    }

    std::cout << "acabé" << std::endl;
    stop(vel);
    publisher -> publish(vel);
    rclcpp::spin_some(node);
    rclcpp::shutdown();
    return 0;


}