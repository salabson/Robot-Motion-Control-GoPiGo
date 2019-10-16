#!/usr/bin/python

import rospy
from std_msgs.msg import Float32

rwheel_motorcmd = Float32()
lwheel_motorcmd = Float32()


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
    lwheel_motorcmd_sub = rospy.Subscriber("/rwheel_motorcmd", Float32, lwheel_motorcmd_callback)
    
    # call the callbacks
    rospy.spin()

