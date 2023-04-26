#include "ros/ros.h"

#include "geometry_msgs/Pose2D.h"
#include "geometry_msgs/Twist.h"
#include"Joystick.h"
#include <string>

int main(int argc, char **argv) {
    ros::init(argc, argv, "joystick_control");
    ros::NodeHandle nh;
    ros::Rate loop_rate(1);

    std::string dev;
    float linearMax = 0;
    float angularMax = 0;
    nh.getParam("/joystick_control/dev", dev);
    nh.getParam("/joystick_control/linearMax", linearMax);
    nh.getParam("/joystick_control/angularMax", angularMax);

    Joystick joystick(dev);
    while (joystick.open() < 0) {
        ROS_INFO("wait for device %s", dev.c_str());
        loop_rate.sleep();
    }

    ros::Publisher twistPublisher = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);

    geometry_msgs::Twist twist;
    int ret = -1;
    while (ros::ok()) {
        ret = joystick.read();

        if (ret > 0) {
            twist.linear.x = -joystick.map.rx * linearMax / XBOX_AXIS_VAL_MAX;
            twist.linear.y = -joystick.map.ry * linearMax / XBOX_AXIS_VAL_MAX;
            twist.angular.z = -joystick.map.lx * angularMax / XBOX_AXIS_VAL_MAX;;
            twist.angular.y = -joystick.map.ly * angularMax / XBOX_AXIS_VAL_MAX;
            twistPublisher.publish(twist);
            ROS_INFO("msg sent");
        }

        ros::spinOnce();

    }
    return 0;
}
