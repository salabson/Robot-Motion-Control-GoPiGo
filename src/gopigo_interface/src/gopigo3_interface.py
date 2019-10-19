#!/usr/bin/python

import rospy
from std_msgs.msg import Float32
from gopigo_interface import Encoder
import gopigo3

class GopigoInterface:
	def __init__(self):
		self.rwheel_motorcmd = Float32()
		self.lwheel_motorcmd = Float32()
		# Create gopigo object driver
		self.GPG = gopigo3.GoPiGo3()
		# Create encoder object
		self encorder_reading = Encoder()
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
		# Reset encoders
		self.GPG.offset_motor_encoder(self.GPG.MOTOR_LEFT, self.get_motor_encoder(self.GPG.MOTOR.LEFT))
		self.GPG.offset_motor_encoder(self.GPG.MOTOR_RIGHT, self.get_motor_encoder(self.GPG.MOTOR.RIGHT))

		rate = rospy.Rate(20)
    		while not rospy.is_shutdown():
			# send power commands to motors
			self.GPG.set_motor_power(self.GPG.MOTOR_RIGHT,self.rwheel_motorcmd.data)
			self.GPG.set_motor_power(self.GPG.MOTOR_LEFT,self.lwheel_motorcmd.data)

			# Reader encoders
			encoder_reading.left = self.GPG.get_motor_encoder(self.GPG.MOTOR_LEFT)
			encoder_reading.right = self.GPG.get_motor_encoder(self.GPG.MOTOR_RIGHT)
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
   
