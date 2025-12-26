#include <ros/ros.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/CommandTOL.h>
#include <mavros_msgs/CommandBool.h>

int main(int argc, char **argv) {
    ros::init(argc, argv, "takeoff_node");
    ros::NodeHandle nh;

    ros::service::waitForService("/mavros/set_mode");
    ros::service::waitForService("/mavros/cmd/arming");
    ros::service::waitForService("/mavros/cmd/takeoff");

    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");
    ros::ServiceClient takeoff_client = nh.serviceClient<mavros_msgs::CommandTOL>("/mavros/cmd/takeoff");

    mavros_msgs::SetMode set_mode_srv;
    set_mode_srv.request.custom_mode = "GUIDED";
    if (set_mode_client.call(set_mode_srv) && set_mode_srv.response.mode_sent) {
        ROS_INFO("GUIDED modu aktif");
    } else {
        ROS_ERROR("mod ayarlanamadı");
        return 1;
    }

    mavros_msgs::CommandBool arm_srv;
    arm_srv.request.value = true;
    if (arming_client.call(arm_srv) && arm_srv.response.success) {
        ROS_INFO("drone motorlari calısti");
    } else {
        ROS_ERROR("motorlar calistirilamadi");
        return 1;
    }

    mavros_msgs::CommandTOL takeoff_srv;
    takeoff_srv.request.altitude = 10.0;
    takeoff_srv.request.latitude = 0;
    takeoff_srv.request.longitude = 0;
    takeoff_srv.request.min_pitch = 0;
    takeoff_srv.request.yaw = 0;

    if (takeoff_client.call(takeoff_srv) && takeoff_srv.response.success) {
        ROS_INFO("takeoff basladi 10 metre");
    } else {
        ROS_ERROR("takeoff basarisiz");
        return 1;
    }

    ros::spin();
    return 0;
}
