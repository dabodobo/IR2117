#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"


int min;
std::shared_ptr<rclcpp::Publisher<std_msgs::msg::Int32>> publisher;

void topic_callback(const std_msgs::msg::Int32::SharedPtr msg){ 
	
    sum += msg->data;
    std_msgs::msg::Int32 out_msg;
    float mean = sum / count;
    out_msg.data = mean;
    publisher->publish(out_msg);
}

int main(int argc, char * argv[]){
    count = 0;
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("min"); //creo el nodo min
    auto subscription = 
        node->create_subscription<std_msgs::msg::Int32>(
            "number", 10, topic_callback); //me suscribo a topic
    publisher = node->create_publisher<std_msgs::msg::Int32>("t_mean", 10); //publico la media

    rclcpp::spin(node); 
    rclcpp::shutdown();
    return 0;
}
