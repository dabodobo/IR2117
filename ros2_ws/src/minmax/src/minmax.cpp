#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include <iostream>


int value;

std::shared_ptr<rclcpp::Publisher<std_msgs::msg::Int32>> publisher;

void min_callback(const std_msgs::msg::Int32::SharedPtr msg){
 
    value = msg->data;    
    std_msgs::msg::Int32 out_msg;
    out_msg.data = value;
    publisher->publish(out_msg);
}

void max_callback(const std_msgs::msg::Int32::SharedPtr msg){
 
    value = msg->data;    
    std_msgs::msg::Int32 out_msg;
    out_msg.data = value;
    publisher->publish(out_msg);
}

int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("minmax"); //creo el nodo minmax
    auto subscription_min = 
        node->create_subscription<std_msgs::msg::Int32>(
            "min", 10, min_callback); //me suscribo a topic min
    
     auto subscription_max = 
        node->create_subscription<std_msgs::msg::Int32>(
            "max", 10, max_callback); //me suscribo a topic min
    
    publisher = node->create_publisher<std_msgs::msg::Int32>("minmax", 10); 

    rclcpp::spin(node); 
    rclcpp::shutdown();
    return 0;
}
