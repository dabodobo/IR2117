#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include <iostream>

int max;
int value;
bool first_read = true;
std::shared_ptr<rclcpp::Publisher<std_msgs::msg::Int32>> publisher;

void topic_callback(const std_msgs::msg::Int32::SharedPtr msg){ 
    value = msg->data;
    if(first_read){
    	max = value;
    	first_read = false;
    }
    if(value > max){
    	max = value;
    }
        
    std_msgs::msg::Int32 out_msg;
    out_msg.data = max;
    publisher->publish(out_msg);
}

int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("max"); //creo el nodo min
    auto subscription = 
        node->create_subscription<std_msgs::msg::Int32>(
            "number", 10, topic_callback); //me suscribo a topic
    publisher = node->create_publisher<std_msgs::msg::Int32>("max", 10); 

    rclcpp::spin(node); 
    rclcpp::shutdown();
    return 0;
}
