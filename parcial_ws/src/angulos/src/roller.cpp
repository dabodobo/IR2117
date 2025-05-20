#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include <cmath>
#include <iostream>
#include <vector>
#include <chrono>
using namespace std::chrono_literals;
double angle;
bool reestablecer = true;
double angle_ref;
std::vector<double> medidas = {M_PI/4,-M_PI/2};

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
void odom_callback(const nav_msgs::msg::Odometry odom){
    angle = quat(odom.pose.pose.orientation.x, odom.pose.pose.orientation.y, odom.pose.pose.orientation.z, odom.pose.pose.orientation.w);
    if (reestablecer){
        angle_ref = angle;
    }
    reestablecer = false;
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

int main(int argc, char ** argv){
    rclcpp::init(argc,argv);
    auto node = rclcpp::Node::make_shared("rolling");
    auto publisher = node -> create_publisher<geometry_msgs::msg::Twist>("cmd_vel",10);
    auto subscriber = node -> create_subscription<nav_msgs::msg::Odometry>("odom",10,odom_callback);
    geometry_msgs::msg::Twist vel;
    rclcpp::WallRate loop_rate(500ms);
    double distancia = angle_dist(angle,angle_ref);
    for (double i : medidas){
        double goal = i;
        std::cout << "voy a girar a " << goal << std::endl;
        while(rclcpp::ok() && distancia < abs(goal)){
            if(abs(goal) > abs(angle)){
                turn_right(vel);
                std::cout << "giro a la derecha" << std::endl;
            }
            else if(abs(goal) < abs(angle)){
                turn_left(vel);
                std::cout << "giro a la izquierda" << std::endl;
            }
            publisher -> publish(vel);
            rclcpp::spin_some(node);
            loop_rate.sleep();
        }
        stop(vel);
        publisher -> publish(vel);
        rclcpp::spin_some(node);
        loop_rate.sleep();
        std::cout << "paro" << std::endl;

    }
    std::cout << "acabé" << std::endl;
    stop(vel);
    publisher -> publish(vel);
    rclcpp::spin_some(node);
    rclcpp::shutdown();
    return 0;


}