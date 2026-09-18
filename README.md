# basic_drone_control_ws

<p>
  <img src="https://img.shields.io/badge/ROS%20Noetic-22314E?style=for-the-badge&logo=ros&logoColor=white" alt="ROS Noetic" />
  <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++" />
  <img src="https://img.shields.io/badge/MAVROS-1F2A44?style=for-the-badge" alt="MAVROS" />
  <img src="https://img.shields.io/badge/ArduPilot%20SITL-161B22?style=for-the-badge" alt="ArduPilot SITL" />
</p>

## Overview

Basic C++ ROS nodes that command a single drone through MAVROS: arm and take off, land, fly to a goal and simple
motion control. They use ArduPilot's GUIDED mode and work with SITL or a real flight controller. Written as building
blocks for later autonomy and swarm work.

**Quick start:** `rosrun basic_drone_control takeoff_node`

## Proje hakkında

Tek bir drone'u MAVROS üzerinden komut seviyesinde yöneten C++ ROS düğümleri. ArduPilot'un GUIDED modu kullanılır;
SITL simülasyonunda ya da gerçek bir uçuş kontrolcüsüyle çalışır. Sonraki otonomi ve sürü çalışmalarına temel olması
için yazıldı.

## Düğümler

| Düğüm | Ne yapar |
|---|---|
| `takeoff_node` | GUIDED moda alır, arm eder ve `cmd/takeoff` ile kalkar |
| `landing_node` | `cmd/land` ile iner, ardından disarm eder |
| `go_to_goal_node` | `local_position/pose` konumunu okuyup `setpoint_velocity/cmd_vel` ile hedefe hız komutu verir |
| `motion_control` | mod değiştirme, arm, kalkış ve hız komutlarıyla temel hareket |
| `drone_control` | `setpoint_position/local` ile drone'u (0, 0, 10) konumuna gönderir |

## Kurulum ve çalıştırma

Gereksinimler: ROS Noetic, MAVROS ve çalışan bir ArduPilot SITL ya da uçuş kontrolcüsü.

```bash
cd basic_drone_control_ws
catkin_make
source devel/setup.bash
```

Önce MAVROS'u başlatın, sonra düğümleri çalıştırın:

```bash
roslaunch mavros apm.launch fcu_url:=udp://127.0.0.1:14550@
rosrun basic_drone_control takeoff_node
rosrun basic_drone_control go_to_goal_node
rosrun basic_drone_control landing_node
```

## Dosya yapısı

```text
basic_drone_control_ws/
└── src/basic_drone_control/
    ├── CMakeLists.txt
    ├── package.xml
    └── src/
        ├── takeoff_node.cpp
        ├── landing_node.cpp
        ├── go_to_goal_node.cpp
        ├── motion_control.cpp
        └── drone_control.cpp
```
