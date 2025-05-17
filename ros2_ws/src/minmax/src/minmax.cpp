#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/int32_multi_array.hpp"
#include <iostream>
#include <vector>



int max;
int min;
int value;
bool first_read = true;
std::shared_ptr<rclcpp::Publisher<std_msgs::msg::Int32MultiArray>> publisher;

void topic_callback(const std_msgs::msg::Int32::SharedPtr msg){
    std::vector<int> minmax;
    value = msg->data;
    
     if(first_read){
     	min = value;
    	max = value;
    	first_read = false;
    }
    if(value > max){
    	max = value;
    }
    if(value < min){
    	min = value;
    }
    
    minmax.push_back(min);
    minmax.push_back(max);
    
    std_msgs::msg::Int32MultiArray out_msg;
    out_msg.data = minmax;
    publisher->publish(out_msg);
}



int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("minmax"); //creo el nodo minmax
    auto subscription_min = 
        node->create_subscription<std_msgs::msg::Int32>(
            "factorial", 10, topic_callback); //me suscribo a topic min
    
    publisher = node->create_publisher<std_msgs::msg::Int32MultiArray>("minmax", 10); 

    rclcpp::spin(node); 
    rclcpp::shutdown();
    return 0;
}
