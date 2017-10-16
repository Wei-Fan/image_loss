#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "std_msgs/Header.h"
#include "std_msgs/Bool.h"
#include <iostream>

#include "ardrone_autonomy/Navdata.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <math.h>

using namespace std;

class ImageLoss
{
public:
    double image_stamp;
    double nav_stamp;

    ImageLoss();
private:
    ros::NodeHandle node;
    ros::Subscriber nav_sub;
    ros::Subscriber image_sub; 

    void navCallback(const ardrone_autonomy::Navdata &msg);
    void imageCallback(const sensor_msgs::Image &msg);
};
ImageLoss::ImageLoss()
{
    image_sub = node.subscribe("/ardrone/front/image_raw",1,&ImageLoss::imageCallback,this);
    nav_sub = node.subscribe("/ardrone/navdata",1,&ImageLoss::navCallback,this);
}
void ImageLoss::imageCallback(const sensor_msgs::Image &msg)
{
    image_stamp = msg.header.stamp.sec;
    //cout << "time of image " << msg.header.stamp.sec << endl;
}
void ImageLoss::navCallback(const ardrone_autonomy::Navdata &msg)
{
    nav_stamp = msg.header.stamp.sec;
    //cout << "time of nav   " << msg.header.stamp.sec << endl;
}
int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_loss");
    ros::NodeHandle n;
    ros::Publisher image_loss_pub = n.advertise<std_msgs::Bool>("/image_loss",1) ;
    ros::Rate loop_rate(10);

    ImageLoss imalo;
    while(ros::ok())
    {
        std_msgs::Bool ret;
        cout << "time" << fabs(imalo.nav_stamp - imalo.image_stamp) << endl;
        if(fabs(imalo.nav_stamp - imalo.image_stamp) > 0.5)
        {
            ret.data = false;
            image_loss_pub.publish(ret);
        }else
        {
            ret.data = true;
            image_loss_pub.publish(ret);
        }
        loop_rate.sleep();
        ros::spinOnce();
    }
    return 0;
}
