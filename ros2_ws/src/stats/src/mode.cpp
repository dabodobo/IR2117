#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/int32.hpp"
#include <map>
#include <algorithm>


std::shared_ptr<rclcpp::Publisher<std_msgs::msg::Float32>> publisher; //declaro el subscriptor para el callback
std::map<float,int> counter;
int mayor = 0;
float valor_mayor = 0.0;
void topic_callback(const std_msgs::msg::Int32::SharedPtr msg){ 
    float num = msg->data;
    counter[num]+=1;
    
    for(std::pair<float,int> i : counter ){
    	if(i.second > mayor){
    		mayor = i.second;
    		valor_mayor = i.first;
    	}
    
    }
    
    std_msgs::msg::Float32 out_msg;
    out_msg.data = valor_mayor;
    publisher->publish(out_msg);
}

int main(int argc, char * argv[]){
	
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("mode"); //creo el nodo mode
    auto subscription = 
        node->create_subscription<std_msgs::msg::Int32>(
            "number", 10, topic_callback); //me suscribo a topic
    publisher = node->create_publisher<std_msgs::msg::Float32>("topic_mode", 10); //publico la media

    rclcpp::spin(node); 
    rclcpp::shutdown();
    return 0;
}


