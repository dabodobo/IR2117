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



bool north =false,northeast = false, northwest=false, south= false, southeast= false, southwest= false, east= false, west= false;

void north_callback(const example_interfaces::msg::Bool::SharedPtr msg){
    north = msg -> data;
}

void northeast_callback(const example_interfaces::msg::Bool::SharedPtr msg){
    northeast = msg -> data;
}

void northwest_callback(const example_interfaces::msg::Bool::SharedPtr msg){
    northwest = msg -> data;
}

void west_callback(const example_interfaces::msg::Bool::SharedPtr msg){
    west= msg -> data;
}
void east_callback(const example_interfaces::msg::Bool::SharedPtr msg){
    east = msg -> data;
}

void south_callback(const example_interfaces::msg::Bool::SharedPtr msg){
    south = msg -> data;
}

void southeast_callback(const example_interfaces::msg::Bool::SharedPtr msg){
    southeast = msg -> data;
}

void southwest_callback(const example_interfaces::msg::Bool::SharedPtr msg){
    southwest = msg -> data;
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
    vel.angular.z = 0.3;
}
void turn_right(geometry_msgs::msg::Twist& vel){
    vel.angular.z = -0.3;
}
void stop(geometry_msgs::msg::Twist& vel){
    vel.angular.z = 0.0;
}

int main(int argc, char ** argv){
    rclcpp::init(argc,argv);
    auto node = rclcpp::Node::make_shared("monitor");
    auto publisher = node -> create_publisher<geometry_msgs::msg::Twist>("/cmd_vel",10);
    auto subscriber = node -> create_subscription<nav_msgs::msg::Odometry>("/odom",10,odom_callback);
    auto sub_north = node -> create_subscription<example_interfaces::msg::Bool>("/north/obstacle",10,north_callback);
    auto sub_west = node -> create_subscription<example_interfaces::msg::Bool>("/west/obstacle",10,west_callback);
    auto sub_east = node -> create_subscription<example_interfaces::msg::Bool>("/east/obstacle",10,east_callback);
    
    auto sub_northeast = node -> create_subscription<example_interfaces::msg::Bool>("/northeast/obstacle",10,northeast_callback);
    auto sub_northwest = node -> create_subscription<example_interfaces::msg::Bool>("/northwest/obstacle",10,northwest_callback);
    auto sub_south = node -> create_subscription<example_interfaces::msg::Bool>("/south/obstacle",10,south_callback);
    
    auto sub_southeast = node -> create_subscription<example_interfaces::msg::Bool>("/southeast/obstacle",10,southeast_callback);
    auto sub_southweast = node -> create_subscription<example_interfaces::msg::Bool>("/southwest/obstacle",10,southwest_callback);
    
    geometry_msgs::msg::Twist vel;
    
    rclcpp::WallRate loop_rate(100ms);

    double goal;
    double distancia;
    bool girar = false;
    while(rclcpp::ok()){
	stop(vel);
        girar = north || northeast || northwest || east || west || south || southwest || southeast;

        if (girar) {
            if(north){
                goal = 0;
            }
            else if(east){
                goal = -M_PI/2;
            }
            else if(west){
                goal = M_PI/2;
            }
             else if(south){
                goal = M_PI;
            }
             else if(northeast){
                goal = -M_PI/4;
            }
             else if(northwest){
                goal = M_PI/4;
            }
             else if(southeast){
                goal = -3*M_PI/4;
            }
             else if(southwest){
                goal = 3*M_PI/4;
            }
            

            distancia = angle_dist(angle, goal);
            std::cout << "Voy a girar hacia " << goal << " (distancia: " << distancia << " rad)" << std::endl;

            while(rclcpp::ok() && std::abs(distancia) > 0.2){
                rclcpp::spin_some(node);
                distancia = angle_dist(angle, goal);

                if(distancia < 0){
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
