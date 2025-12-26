#include <ros/ros.h>
#include <mavros_msgs/CommandTOL.h>
#include <mavros_msgs/CommandBool.h>

int main(int argc, char **argv) {
    ros::init(argc, argv, "landing_node");
    ros::NodeHandle nh;

    ros::service::waitForService("/mavros/cmd/land");
    ros::service::waitForService("/mavros/cmd/arming");

    ros::ServiceClient land_client = nh.serviceClient<mavros_msgs::CommandTOL>("/mavros/cmd/land");
    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");

    mavros_msgs::CommandTOL land_srv;
    land_srv.request.min_pitch = 0;
    land_srv.request.yaw = 0;
    land_srv.request.latitude = 0.0;
    land_srv.request.longitude = 0.0;
    land_srv.request.altitude = 0;

    if (land_client.call(land_srv) && land_srv.response.success) {
        ROS_INFO("landing basladi");
    } else {
        ROS_ERROR("landing başarısız");
        return 1;
    }

    ros::Duration(30.0).sleep();

    mavros_msgs::CommandBool arm_srv;
    arm_srv.request.value = false;

    if (arming_client.call(arm_srv) && arm_srv.response.success) {
        ROS_INFO("drone motorlari durduruldu");
    } else {
        ROS_ERROR("motorlar durdurulamadi");
        return 1;
    }

    return 0;
}
