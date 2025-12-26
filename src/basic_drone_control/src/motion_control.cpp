#include <ros/ros.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/CommandTOL.h>
#include <geometry_msgs/TwistStamped.h>

// Global değişkenler
mavros_msgs::State current_state;
bool state_received = false; // State mesajının gelip gelmediğini kontrol için

// Uçuş durumu callback
void state_cb(const mavros_msgs::State::ConstPtr& msg) {
    current_state = *msg;
    // Sadece ilk bağlantıda veya durum değiştiğinde bilgi ver
    if (!state_received) {
        ROS_INFO("State Callback Tetiklendi! Mod: %s, Connected: %d",
                 current_state.mode.c_str(), current_state.connected);
        state_received = true;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "motion_control_debug");
    ros::NodeHandle nh;

    ROS_INFO(">> NODE BASLATILDI <<");

    // Subscriber ve Publisher
    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("mavros/state", 10, state_cb);
    ros::Publisher velocity_pub = nh.advertise<geometry_msgs::TwistStamped>("mavros/setpoint_velocity/cmd_vel", 10);

    // Servisler
    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");
    ros::ServiceClient takeoff_client = nh.serviceClient<mavros_msgs::CommandTOL>("mavros/cmd/takeoff");

    ros::Rate rate(20.0);

    // 1. ADIM: FCU BAĞLANTISI KONTROLÜ
    ROS_INFO("1. FCU (Flight Controller) baglantisi bekleniyor...");
    while (ros::ok() && !current_state.connected) {
        ros::spinOnce();
        rate.sleep();
        // Her 2 saniyede bir ekrana yaz (takılıp takılmadığını anlamak için)
        ROS_INFO_THROTTLE(2, "Hala FCU baglantisi bekleniyor... (Mavros calisiyor mu?)");
    }
    ROS_INFO("FCU BAGLANDI! Guncel Mod: %s", current_state.mode.c_str());

    // Velocity mesajı hazırlığı
    geometry_msgs::TwistStamped vel_msg;
    vel_msg.twist.linear.x = 0.0;
    vel_msg.twist.linear.y = 0.0;
    vel_msg.twist.linear.z = 0.0;

    // 2. ADIM: SETPOINT YAYINI
    ROS_INFO("2. Setpoint yayini basladi (100 dongu)...");
    for (int i = 0; i < 100 && ros::ok(); i++) {
        vel_msg.header.stamp = ros::Time::now();
        velocity_pub.publish(vel_msg);
        ros::spinOnce();
        rate.sleep();
    }
    ROS_INFO("Setpoint yayini tamamlandi.");

    // 3. ADIM: ARMING
    ROS_INFO("3. Arming istegi gonderiliyor...");
    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value = true;

    // Arm servisini çağır
    if (arming_client.call(arm_cmd)) {
        if (arm_cmd.response.success) {
            ROS_INFO("ARM BASARILI!");
        } else {
            ROS_ERROR("ARM REDDEDILDI! (GPS 3D Fix var mi? Batarya takili mi?) Result: %d", arm_cmd.response.result);
            // Hata olsa bile devam etmeyi dene ya da return at. Debug için devam ediyoruz.
        }
    } else {
        ROS_ERROR("ARM Servisine ulasilamadi!");
    }

    // 4. ADIM: MODE DEGISTIRME (GUIDED)
    ROS_INFO("4. GUIDED moda geciliyor...");
    mavros_msgs::SetMode guided_set_mode;
    guided_set_mode.request.custom_mode = "GUIDED";

    if (set_mode_client.call(guided_set_mode)) {
        if (guided_set_mode.response.mode_sent) {
            ROS_INFO("GUIDED Modu aktif!");
        } else {
            ROS_ERROR("GUIDED Mod istegi basarisiz oldu!");
        }
    } else {
        ROS_ERROR("SetMode servisine ulasilamadi!");
    }

    // 5. ADIM: TAKEOFF
    ROS_INFO("5. Takeoff (10m) istegi gonderiliyor...");
    mavros_msgs::CommandTOL takeoff_srv;
    takeoff_srv.request.altitude = 10.0;
    takeoff_srv.request.latitude = 0;
    takeoff_srv.request.longitude = 0;
    takeoff_srv.request.min_pitch = 0;
    takeoff_srv.request.yaw = 0;

    if (takeoff_client.call(takeoff_srv)) {
        if (takeoff_srv.response.success) {
            ROS_INFO("Takeoff komutu kabul edildi.");
        } else {
            ROS_ERROR("Takeoff komutu reddedildi!");
        }
    } else {
        ROS_ERROR("Takeoff servisine ulasilamadi!");
    }

    ROS_INFO("Yukselmek icin 10 saniye bekleniyor...");
    ros::Duration(10.0).sleep();

    // 6. ADIM: HAREKETLER
    // İleri
    ROS_INFO("6. Ileri hareket basliyor...");
    ros::Time start = ros::Time::now();
    while (ros::ok() && (ros::Time::now() - start < ros::Duration(5.0))) {
        vel_msg.header.stamp = ros::Time::now();
        vel_msg.twist.linear.x = 1.0;
        velocity_pub.publish(vel_msg);
        ros::spinOnce();
        rate.sleep();
        ROS_INFO_THROTTLE(1, "Hiz: 1.0 m/s (Ileri gidiliyor...)");
    }

    ROS_INFO("Gorev Tamamlandi.");
    return 0;
}
