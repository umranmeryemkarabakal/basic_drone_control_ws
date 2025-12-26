#include <ros/ros.h>
#include <geometry_msgs/TwistStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <cmath>
#include <iostream>

class DroneController {
private:
    ros::NodeHandle nh_;
    ros::Publisher velocity_pub_;
    ros::Subscriber position_sub_;

    geometry_msgs::PoseStamped current_pose_;
    double target_x_, target_y_, target_z_;

public:
    DroneController() : target_x_(0), target_y_(0), target_z_(0) {
        // Publisher ve Subscriber tanımları
        velocity_pub_ = nh_.advertise<geometry_msgs::TwistStamped>("/mavros/setpoint_velocity/cmd_vel", 10);
        position_sub_ = nh_.subscribe("/mavros/local_position/pose", 10, &DroneController::positionCallback, this);

        ROS_INFO("Drone controller initialized!");
    }

    // Callback fonksiyonu: Konum verisini günceller
    void positionCallback(const geometry_msgs::PoseStamped::ConstPtr& msg) {
        current_pose_ = *msg;
    }

    void setTargetCoordinates(double x, double y, double z) {
        target_x_ = x;
        target_y_ = y;
        target_z_ = z;
    }

    void moveToTarget() {
        ros::Rate rate(10); // 10 Hz

        while (ros::ok()) {
            // ÖNEMLİ: C++'da callback'lerin tetiklenmesi için bunu çağırmalıyız
            ros::spinOnce();

            double dx = target_x_ - current_pose_.pose.position.x;
            double dy = target_y_ - current_pose_.pose.position.y;
            double dz = target_z_ - current_pose_.pose.position.z;

            double distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2) + std::pow(dz, 2));

            ROS_INFO("Distance to target: %.2f", distance);

            if (distance < 0.5) {
                ROS_INFO("Target reached!");
                // Hedefe varınca durması için hızları sıfırlayalım (Opsiyonel ama güvenli)
                geometry_msgs::TwistStamped stop_cmd;
                velocity_pub_.publish(stop_cmd);
                break;
            }

            geometry_msgs::TwistStamped cmd;
            // Python kodundaki mantık: Hız vektörü oluştur (Sabit 0.5 m/s hızla hedefe yönel)
            cmd.twist.linear.x = 0.5 * (dx / distance);
            cmd.twist.linear.y = 0.5 * (dy / distance);
            cmd.twist.linear.z = 0.5 * (dz / distance);

            // Zaman damgasını güncelle
            cmd.header.stamp = ros::Time::now();

            velocity_pub_.publish(cmd);
            rate.sleep();
        }
    }
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "go_to_goal_node_cpp");

    DroneController controller;

    // Bağlantıların oturması için kısa bir bekleme
    ros::Duration(1.0).sleep();

    while (ros::ok()) {
        double x, y, z;

        std::cout << "Enter target X: ";
        if (!(std::cin >> x)) break; // Giriş hatası kontrolü (örn: CTRL+C)

        std::cout << "Enter target Y: ";
        std::cin >> y;

        std::cout << "Enter target Z: ";
        std::cin >> z;

        controller.setTargetCoordinates(x, y, z);
        controller.moveToTarget();

        ROS_INFO("Target reached! Please enter new target coordinates");
    }

    return 0;
}
