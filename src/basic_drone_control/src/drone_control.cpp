#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>

int main(int argc, char **argv) {
    ros::init(argc, argv, "drone_control");
    ros::NodeHandle nh;

    ros::Publisher pose_pub = nh.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_position/local", 10);
    ros::Rate rate(20.0);

    geometry_msgs::PoseStamped pose;
    pose.pose.position.x = 0;
    pose.pose.position.y = 0;
    pose.pose.position.z = 10;

    while (ros::ok()) {
        pose.header.stamp = ros::Time::now();
        pose_pub.publish(pose);
        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
