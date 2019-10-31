# Robot-Motion-Control-GoPiGo
<p align="center">
  <img  src="https://github.com/salabson/Robot-Motion-Control-GoPiGo/blob/openloop/gopi3.jpg">
</p>

This project implemented the **open loop control** on an actual robot (GoPiGo Differential Drive Mobile Robot). We will transform linear and rotational velocities target into motor commands that will be executed by robot's left and right wheels. we will use open loop control system that does not use sensor like encoders for feedback.
we need two machines; a host machine running ubuntu and ROS and a Gopigo robot with the Ubiquity Robotics Raspberry Pi images installed which is based on Ubuntu 16.04 and is pre-installed with ROS.
These two machines communicate via ROS messages by networking together to share the same ROS master. The node **gopigo_interface.py** on the Raspberry Pi control the motors while nodes **diffdrive_controller** and **base_controller** on the host machine compute target velocities and motor commands for the motors.

### Compatability: 
#### Software
- ROS: Kinetic
- OS: Ubuntu 16.04
- ubiquity Robotics Raspberry Pi image
#### Hardware
- Differential Drive Robot: GoPiGo3
- Laptop



### Architecture
<p align="center">
  <img  src="https://github.com/salabson/Robot-Motion-Control-GoPiGo/blob/openloop/art.png">
</p>

### How to clone this repository
```
$ cd ~/catkin_ws/src
~/catkin_ws/src$ git clone -b https://github.com/salabson/Robot-Motion-Control-GoPiGo
```
**Note:** The gopigo_interface controls the robot's motors, therefore, copy or move gopigo_interface package to Raspberry Pi
using ssh with the following instructions and make sure your src folder in your catkin_ws.

```
~/catkin_ws/src$ scp -r gopigo_interface ubuntu@ubuquityrobot:~/catkin_ws/src
```
### Build catkin workspace and source development folder
```
~/catkin_ws/src$ cd ~/catkin_ws
~/catkin_ws$ catkin_make
~/catkin_ws$ source devel/setup.bash
```
### Usage

**Termainal 1 on host**
```
$ cd ~/catkin_ws
~/catkin_ws$ souce devel/setup.bash
~/catkin_ws$ rosrun diffdrive_controller diffdrive_controller
```


**Termainal 2 on host**
```
$ cd ~/catkin_ws
~/catkin_ws$ souce devel/setup.bash
~/catkin_ws$ rosrun base_controller base_controller
```

**Termainal 3 on host**
```
$ cd ~/catkin_ws
~/catkin_ws$ souce devel/setup.bash
~/catkin_ws$ rosrun teleop_twist_keyboard teleop_twist_keyboard.py
```

**Termainal 4 on Raspberry Pi: connect using ssh and run below commands**
```
$ cd ~/catkin_ws
~/catkin_ws$ catkin_make
~/catkin_ws$ souce devel/setup.bash
~/catkin_ws$ rosrun gopigo_interface gopigo_interface.py
```
Finally, make sure the terminal 3 that you run teleop_twist_keyboard is active and then press appropriate keys on keyboard to move the robot accordingly
