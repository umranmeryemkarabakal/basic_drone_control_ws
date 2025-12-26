# Temel Drone Kontrol Node'ları (ROS / MAVROS)

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
