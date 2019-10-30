# Robot-Motion-Control-GoPiGo
<p align="center">
  <img  src="https://github.com/salabson/Robot-Motion-Control-GoPiGo/blob/openloop/gopi3.jpg">
</p>

This project implemented the **open loop control** on an actual robot (GoPiGo Differential Drive Mobile Robot). We will transform linear and rotational velocities target into motor commands that will be executed by robot's left and right wheels. we will use open loop control system that does not use sensor like encoders for feedback.
we need two machines; a host machine running ubuntu and ROS and a robot with the Ubiquity Robotics Raspberry Pi images installed which is based on Ubuntu 16.04 and is pre-installed with ROS.
These two machines communicate via ROS messages by networking together share the same ROS master. The node **gopigo_interface.py** on the Raspberry Pi control the motors while nodes **diffdrive_controller** and **base_controller** on the host machine compute target velocities and motor commands for the motors.

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
