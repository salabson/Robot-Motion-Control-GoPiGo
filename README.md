# Robot-Motion-Control-GoPiGo
This project implemented **open loop control** on an actual robot (GoPiGo Differential Drive Mobile Robot). We will transform 
linear and rotational velocities target into motor commands that will be executed by robot's left and right wheels. we will 
use open loop control system that does not sensor like encoders for feedback.
we need two machines; a host machine ubuntu and ROS and a robot with the Ubiquity Robotics Raspberry Pi images installed which 
is based on Ubuntu 16.04 and is pre-installed with ROS.
These two machines communicate via ROS messages by networking together share the same ROS master. The node **gopigo_interface.py**
on the Raspberry Pi control the motors while nodes **diffdrive_controller** and **base_controller** on the host machine compute target velocities motor commands for the motors.
