#!/usr/bin/python

import rospy
from std_msgs.msg import Float32
import gopigo3

class GopigoInterface:
	def __init__(self):
		self.rwheel_motorcmd = Float32()
		self.lwheel_motorcmd = Float32()
		# Create gopigo object driver
		self.GPG = gopigo3.GoPiGo3()
		# Create subscriber objects
    		self.rwheel_motorcmd_sub = rospy.Subscriber("/rwheel_motorcmd", Float32, self.rwheel_motorcmd_callback)
    		self.lwheel_motorcmd_sub = rospy.Subscriber("/lwheel_motorcmd", Float32, self.lwheel_motorcmd_callback)

	
	def rwheel_motorcmd_callback(self, msg):
    		self.rwheel_motorcmd.data = msg.data
    		rospy.loginfo("rmotor_cmd: %f", self.rwheel_motorcmd.data)

	
	def lwheel_motorcmd_callback(self, msg):
    		self.lwheel_motorcmd.data = msg.data
    		rospy.loginfo("lmotor_cmd: %f",self.lwheel_motorcmd.data)
	
	def spin(self):
		rate = rospy.Rate(20)
    		while not rospy.is_shutdown():
			self.GPG.set_motor_power(self.GPG.MOTOR_RIGHT,self.rwheel_motorcmd.data)
			self.GPG.set_motor_power(self.GPG.MOTOR_LEFT,self.lwheel_motorcmd.data)
			rate.sleep()
    
    		# call the callbacks
   		rospy.spin()



if __name__ == '__main__' :
    
    # Initialize ros system
    rospy.init_node("gopigo_interface")
    
    # Create the class instance
    gopigo_interface = GopigoInterface()
    
    # Run the main program
    gopigo_interface.spin()   
   
