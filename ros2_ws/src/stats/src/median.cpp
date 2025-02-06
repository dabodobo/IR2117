#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/int32.hpp"
#include <vector>


std::shared_ptr<rclcpp::Publisher<std_msgs::msg::Float32>> publisher;
std::vector<float> numeros;
void topic_callback(const std_msgs::msg::Int32::SharedPtr msg){ 
	
    float num = msg->data;
    
    std_msgs::msg::Float32 out_msg;
    
    out_msg.data = mean;
    publisher->publish(out_msg);
}

int main(int argc, char * argv[]){
    count = 0;
	
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("median"); //creo el nodo media
    auto subscription = 
        node->create_subscription<std_msgs::msg::Int32>(
            "number", 10, topic_callback); //me suscribo a topic
    publisher = node->create_publisher<std_msgs::msg::Float32>("topic_median", 10); //publico la media

    rclcpp::spin(node); 
    rclcpp::shutdown();
    return 0;
}

