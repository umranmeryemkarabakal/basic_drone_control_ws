# Temel Drone Kontrol Node'ları (ROS / MAVROS)

<p>
  <img src="https://img.shields.io/badge/ROS%20Noetic-22314E?style=for-the-badge&logo=ros&logoColor=white" alt="ROS Noetic" />
  <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++" />
  <img src="https://img.shields.io/badge/MAVROS-1F2A44?style=for-the-badge" alt="MAVROS" />
  <img src="https://img.shields.io/badge/ArduPilot%20SITL-161B22?style=for-the-badge" alt="ArduPilot SITL" />
</p>

## Overview

Basic ROS / MAVROS command nodes for a single drone in C++: arm and take off, land, go to a goal point and simple motion control. Meant as building blocks for higher-level autonomy.

**Quick start:** `rosrun basic_drone_control takeoff_node`

## Proje hakkında

Bu depo (repository), ROS ve MAVROS kullanılarak uygulanmış temel drone kontrol node'larını (düğümlerini) içerir.
Bu node'lar; kalkış, iniş ve hedefe gitme gibi temel uçuş komutlarını sağlar. Üst seviye otonomi ve sürü sistemleri için birer yapı taşı olarak tasarlanmışlardır.

Bu çalışma alanı, tek başına çalışan otonom bir sistemden ziyade; öğrenme, deney yapma ve entegrasyon amacıyla oluşturulmuştur.

## Genel Bakış

Bu çalışma alanı, simülasyonda veya gerçek donanım üzerinde bir drone'u kontrol etmek için MAVROS ile etkileşime giren basit, komut seviyesindeki ROS node'larını içerir.

**Uygulanan Yetenekler:**

* Kalkış (Takeoff)
* İniş (Landing)
* Hedefe gitme navigasyonu (Go-to-goal)
* Temel hareket kontrolü
* Merkezi kontrol mantığı

## Node'lar (Düğümler)

| Node                    | Açıklama                                                              |
| :---------------------- | :---------------------------------------------------------------------- |
| `takeoff_node.cpp`    | İHA'yı "arm" eder (motorları aktifleştirir) ve kalkış komutu verir. |
| `landing_node.cpp`    | Kontrollü bir iniş komutu verir.                                      |
| `go_to_goal_node.cpp` | Konum/hedef komutları gönderir.                                       |
| `motion_control.cpp`  | Temel hareketle ilgili komutları yönetir.                             |
| `drone_control.cpp`   | Çekirdek kontrol mantığı ve MAVROS etkileşimi.                     |

## Ön Gereksinimler

Herhangi bir node çalıştırılmadan önce aşağıdakiler mutlaka çalışıyor olmalıdır:

* ROS (Noetic)
* MAVROS
* Bağlı bir uçuş kontrolcüsü veya simülatör (ArduPilot / PX4)


## Derleme Talimatları (Build)

```bash
cd basic_drone_control_ws
catkin_make
source devel/setup.bash
```

## Kullanım

Önce MAVROS'u çalıştırın.

Örnek çalıştırma:

```bash
rosrun basic_drone_control takeoff_node
rosrun basic_drone_control go_to_goal_node
rosrun basic_drone_control landing_node
```
