#include<ros/ros.h>
#include<std_msgs/Float32.h>
#include<geometry_msgs/Twist.h>



class DiffDrive{
	private:
		std_msgs::Float32 rwheel_tangential_vel_target;
		std_msgs::Float32 lwheel_tangential_vel_target;
		
		//Distance between the wheels
		double robot_width;

	        // Create subscriber object
       		ros::Subscriber cmd_vel_sub;
        
        	// Create publisher objects
        	ros::Publisher  rwheel_tang_vel_target_pub; 
        	ros::Publisher  lwheel_tang_vel_target_pub; 
		
	public:
		//Create class constructor
		DiffDrive(ros::NodeHandle *nh){

			robot_width=0.14;
			
			//Initialize subscriber object
        		cmd_vel_sub = nh->subscribe("/cmd_vel", 10, &DiffDrive::cmd_vel_callback, this);
        
        		//Initialize publisher objects
       			rwheel_tang_vel_target_pub = nh->advertise<std_msgs::Float32>("/rwheel_tangential_vel_target", 10); 
        		lwheel_tang_vel_target_pub = nh->advertise<std_msgs::Float32>("/lwheel_tangential_vel_target", 10); 
		}
        
		
		// Velocity callback
		void cmd_vel_callback(const geometry_msgs::Twist::ConstPtr& twist_msg){
			double vel_x = twist_msg->linear.x;
			double vel_th = twist_msg->angular.z;
		
			// right and left wheel tangential velocities
			double vr = 0.0;
			double vl = 0.0;

			if(vel_x == 0.0){
				// robot turning in place
				vr = vel_th*robot_width/2.0;
				vl = -1*vr;
			} else if(vel_th == 0.0){
				// robot moving forward/backward
				vr = vl = vel_x;
			}else {
				// robot moving in arc: both vel_x and vel_th are zero
				vr = vel_x + vel_th*robot_width/2.0;
				vl = vel_x - vel_th*robot_width/2.0;

			}

			rwheel_tangential_vel_target.data = vr;
			lwheel_tangential_vel_target.data = vl;
		}


		void spin(){

			ros::Rate rate(20);
        
        		// start publishing
        		while(ros::ok()){
				rwheel_tang_vel_target_pub.publish(rwheel_tangential_vel_target);
				lwheel_tang_vel_target_pub.publish(lwheel_tangential_vel_target);
				ros::spinOnce();
				rate.sleep();
	       		 }

		}



};


int main(int argc, char** argv){
        // Initialize ROS system and create node object
        ros::init(argc, argv,"diffdrive");
        ros::NodeHandle nh;

        // Create class instance
	DiffDrive diffdrive = DiffDrive(&nh);
	
	//Run the main program
	diffdrive.spin();
        
        return 0;

}
