#!/usr/bin/python

import rospy
from std_msgs.msg import Float32
import gopigo3

rwheel_motorcmd = Float32()
lwheel_motorcmd = Float32()

GPG = gopigo3.GoPiGo3()

def rwheel_motorcmd_callback(msg):
    rwheel_motorcmd.data = msg.data
    rospy.loginfo("rmotor_cmd: %f", rwheel_motorcmd.data)

def lwheel_motorcmd_callback(msg):
    lwheel_motorcmd.data = msg.data
    rospy.loginfo("lmotor_cmd: %f",lwheel_motorcmd.data)



if __name__ == '__main__' :
    
    # Initialize ros system
    rospy.init_node("gopigo_interface")
    
    # Create subscriber objects
    rwheel_motorcmd_sub = rospy.Subscriber("/rwheel_motorcmd", Float32, rwheel_motorcmd_callback)
    lwheel_motorcmd_sub = rospy.Subscriber("/lwheel_motorcmd", Float32, lwheel_motorcmd_callback)
    
    rate = rospy.Rate(20)
    while not rospy.is_shutdown():
	GPG.set_motor_power(GPG.MOTOR_RIGHT,rwheel_motorcmd.data)
	GPG.set_motor_power(GPG.MOTOR_LEFT,lwheel_motorcmd.data)
	rate.sleep()
    
    # call the callbacks
    rospy.spin()

