#include <ros/ros.h>
#include <std_msgs/String.h>

extern "C"
{
    #include "sonar_message/uart.h"
}

void stringCallBack(const std_msgs::String &msg)
{
    ROS_INFO("Subcribe string:%s", msg.data.c_str());
    uart_write((char*)msg.data.c_str(), msg.data.length());
}

int main(int argc,char **argv)
{
    ros::init(argc, argv, "sonar_message_relayer");

    ros::NodeHandle nh;
    char buf[101];
    if(uart_open("/dev/ttyUSB0", 8, 115200, 'N', 1))
    {
        ROS_ERROR("Error occurs when open uart!");
        return 1;
    }

    ros::Publisher pub = nh.advertise<std_msgs::String>("sender",100);
    ros::Subscriber sub = nh.subscribe("/sonar_message_receiver", 100, stringCallBack);

    ros::Rate loop_rate(100);

    int count = 0;
    while(ros::ok())
    {
        int size = uart_read(buf, 100);
        if(size > 0)
        {
            buf[size] = '\0';
            std_msgs::String msg;
            msg.data = buf;
            pub.publish(msg);
        }
        ros::spinOnce();
        loop_rate.sleep();
    }
    uart_close();
    return 0;
}
