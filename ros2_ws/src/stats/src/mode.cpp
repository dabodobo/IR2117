#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/int32_multi_array.hpp"
#include <map>
#include <vector>
#include <algorithm>


std::shared_ptr<rclcpp::Publisher<std_msgs::msg::Int32MultiArray>> publisher; //declaro el subscriptor para el callback
std::map<float,int> counter;

void topic_callback(const std_msgs::msg::Int32::SharedPtr msg){ 
  
    float num = msg->data;
    counter[num]+=1;
    int mayor = 0;
    std::vector<int> mayor_aparicion; //vector que voy a publicar
    
    for(std::pair<int,int> i : counter ){
    	if(i.second > mayor){
    		mayor = i.first;
    	}
    
    }
    
    for(std::pair<int,int> i : counter ){
    	if(i.second == mayor){
    		mayor_aparicion.push_back(i.first);
    	}
    
    }
    
    std_msgs::msg::Int32MultiArray out_msg;
    
    out_msg.data = mayor_aparicion;
    publisher->publish(out_msg);
}

int main(int argc, char * argv[]){
	
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("mode"); //creo el nodo mode
    auto subscription = 
        node->create_subscription<std_msgs::msg::Int32>(
            "factorial", 10, topic_callback); //me suscribo a topic
    publisher = node->create_publisher<std_msgs::msg::Int32MultiArray>("topic_mode", 10); //publico la media

    rclcpp::spin(node); 
    rclcpp::shutdown();
    return 0;
}


