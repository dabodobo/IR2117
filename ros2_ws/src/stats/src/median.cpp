#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/int32.hpp"
#include <vector>
#include <algorithm>


std::shared_ptr<rclcpp::Publisher<std_msgs::msg::Float32>> publisher; //declaro el subscriptor para el callback
std::vector<float> numeros;
void topic_callback(const std_msgs::msg::Int32::SharedPtr msg){ 

    
    float median;
    std_msgs::msg::Float32 out_msg;
    
    float num = msg->data;
    numeros.push_back(num);
    std::sort(numeros.begin(),numeros.end());
    
   if(numeros.size() % 2 != 0){
    median = numeros[numeros.size()/2];
    }
    else{
    	median = (numeros[numeros.size()/2] + numeros[(numeros.size()/2)-1]) / 2;
    }
    out_msg.data = median;
    publisher->publish(out_msg);
}

int main(int argc, char * argv[]){
	
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("median"); //creo el nodo media
    auto subscription = 
        node->create_subscription<std_msgs::msg::Int32>(
            "factorial", 10, topic_callback); //me suscribo a topic
    publisher = node->create_publisher<std_msgs::msg::Float32>("topic_median", 10); //publico la media

    rclcpp::spin(node); 
    rclcpp::shutdown();
    return 0;
}


