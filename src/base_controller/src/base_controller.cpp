#include<ros/ros.h>
#include<std_msgs/Float32.h>
#include<math.h>


class BaseController{
	
	private:
		std_msgs::Float32 rwheel_motorcmd;
		std_msgs::Float32 lwheel_motorcmd;

		double rwheel_tangential_vel_target;
		double lwheel_tangential_vel_target;
		
		double rwheel_angular_vel_enc;
		double lwheel_angular_vel_enc;

		double wheel_radius;

		double max_ang_vel;
		double min_ang_vel;
		double max_motor_cmd;
		double min_motor_cmd;

		// Create subscribers objects
        	ros::Subscriber rwheel_tang_vel_target_sub;
        	ros::Subscriber lwheel_tang_vel_target_sub;
        	ros::Subscriber rwheel_angular_vel_enc_sub;
        	ros::Subscriber lwheel_angular_vel_enc_sub;

      		 // Create publishers objects
        	 ros::Publisher rwheel_motorcmd_pub;  
        	 ros::Publisher lwheel_motorcmd_pub;
		
		//pid parameters	
		 ros::Time current_timestamp;
		 ros::Time last_timestamp;
		 double last_error;
		 double error_sum;
		 double kp;
		 double ki;
		 double kd;
		 bool pid_on;
	public:
		BaseController(ros::NodeHandle *nh){

			wheel_radius = 0.03;

			max_ang_vel = 5.0;
			min_ang_vel = 1.90;
			max_motor_cmd = 100;
			min_motor_cmd = 43;

			 //Initialize subscribers objects
       			  rwheel_tang_vel_target_sub = nh->subscribe("/rwheel_tangential_vel_target",10,
									&BaseController::rwheel_tang_vel_callback,this); 
        		  lwheel_tang_vel_target_sub = nh->subscribe("/lwheel_tangential_vel_target",10,
									&BaseController::lwheel_tang_vel_callback,this);
			  rwheel_angular_vel_enc_sub = nh->subscribe("/rwheel_angular_vel_enc",10,
						&BaseController::rwheel_angular_vel_enc_callback,this);
			  lwheel_angular_vel_enc_sub = nh->subscribe("/lwheel_angular_vel_enc",10,
									&BaseController::lwheel_angular_vel_enc_callback,this);

        		// Initialize publishers objects
        		rwheel_motorcmd_pub = nh->advertise<std_msgs::Float32>("/rwheel_motorcmd",10);  
        		lwheel_motorcmd_pub = nh->advertise<std_msgs::Float32>("/lwheel_motorcmd",10); 
			 
			// initialize pid parameters
			current_timestamp = ros::Time::now();
			last_timestamp = ros::Time::now();
			last_error = 0.0;
			error_sum = 0.0;
			kp = 0.1;
			ki = 0.0;
			kd = 0.0;
			pid_on = true;
		
			
		}
		
		// right wheel tangential velocity callback
		void rwheel_tang_vel_callback(const std_msgs::Float32::ConstPtr& rtang_vel){
			rwheel_tangential_vel_target = rtang_vel->data;
			ROS_INFO("rtan: %f",rwheel_tangential_vel_target);
		    
		}

		// left wheel tangential velocity callback
		void lwheel_tang_vel_callback(const std_msgs::Float32::ConstPtr& ltang_vel){
			lwheel_tangential_vel_target= ltang_vel->data;
			ROS_INFO("ltan: %f",lwheel_tangential_vel_target );

		}

		// right wheel encoder angular  velocity callback
		void rwheel_angular_vel_enc_callback(const std_msgs::Float32::ConstPtr& rwheel_vel_enc){
			
			rwheel_angular_vel_enc = rwheel_vel_enc->data;
		}
		
		
		// left wheel encoder angular  velocity callback
		void lwheel_angular_vel_enc_callback(const std_msgs::Float32::ConstPtr& lwheel_vel_enc){
			
			lwheel_angular_vel_enc = lwheel_vel_enc->data;
		}

		// compute angular velocity
		double tangential_2_angular_vel(double tang_vel){
		       return tang_vel/wheel_radius;

		}

		// This method map angular velocity to motor command
		double angular_2_motor_cmd(double angular_vel){
			if(angular_vel == 0) return 0;

			double slope = (max_motor_cmd-min_motor_cmd)/(max_ang_vel-min_ang_vel);
			double intercept = max_motor_cmd - slope*max_ang_vel;
			double motor_cmd;
		       // positive angular velocity
		       if(angular_vel >0){
			       motor_cmd = slope*angular_vel+intercept;
			       
			       // clip motor command to maximum motor command
			       if(motor_cmd > max_motor_cmd){
				       motor_cmd = max_motor_cmd;
			       }
			       
			       // clip motor command to minimum motor command
			       if(motor_cmd< min_motor_cmd){
				       motor_cmd = min_motor_cmd;
			       }
			       
			       ROS_INFO("motor_cmd p: %f",motor_cmd);
			       return motor_cmd;
		
		       }

		       // negative angular velocity
			if(angular_vel < 0){
			       motor_cmd = slope*abs(angular_vel)+intercept;
			       
			       // clip motor command to maximum motor command
			       if(motor_cmd > max_motor_cmd){
				       motor_cmd = max_motor_cmd;
			       }
			       
			       // clip motor command to minimum motor command
			       if(motor_cmd < min_motor_cmd){
				       motor_cmd = min_motor_cmd;
			       }

			       motor_cmd = -motor_cmd;

			       return motor_cmd;
		       }


		}

		// PID control
		double pid(double target_angular_vel, double measured_angular_vel){
			current_timestamp = ros::Time::now();
			double delta_time = (current_timestamp-last_timestamp).toSec();
			//delta time is zero
			if(delta_time == 0) return 0;

			//calculate proportional error
			double error = target_angular_vel - measured_angular_vel;
			double p = kp*error;
			
			// calculate integral error
			error_sum+=error*delta_time;
			double i = ki*error_sum;

			// calculate derivative error
			double delta_error = error - last_error;
			double d = kd*delta_error/delta_time;
			
			double control_signal = p+i+d;
			
			// save the current data for next iteration
			last_timestamp = current_timestamp;
			last_error = error;
			
			return control_signal;
		

		
		}

		// Compute  motor command
		double  calculate_motorcmd(double tang_vel){
			double angular_vel = tangential_2_angular_vel(tang_vel);
			return angular_2_motor_cmd(angular_vel);
		
		}
		
		void update_lwheel(){
			double angular_vel_target = tangential_2_angular_vel(lwheel_tangential_vel_target);
			if(pid_on){
				double angular_vel_target_pid =pid(angular_vel_target, lwheel_angular_vel_enc);
				lwheel_motorcmd.data = angular_2_motor_cmd(angular_vel_target_pid);
			}else{
				
				lwheel_motorcmd.data = angular_2_motor_cmd(angular_vel_target);
                             }
			lwheel_motorcmd_pub.publish(lwheel_motorcmd);

		}
		

		void update_rwheel(){
                        double angular_vel_target = tangential_2_angular_vel(rwheel_tangential_vel_target);
                        if(pid_on){
                                double angular_vel_target_pid =pid(rwheel_angular_vel_enc, lwheel_angular_vel_enc);
                                rwheel_motorcmd.data = angular_2_motor_cmd(angular_vel_target_pid);
                        }else{

                                rwheel_motorcmd.data = angular_2_motor_cmd(angular_vel_target);
                             }
                        rwheel_motorcmd_pub.publish(rwheel_motorcmd);

                }

		void spin(){
			ros::Rate rate(20);

        		//start publishing
        		while(ros::ok()){ 
				ROS_INFO("rtan: %f ltan: %f", rwheel_tangential_vel_target, lwheel_tangential_vel_target);
				update_lwheel();
				update_rwheel();
				ros::spinOnce();
				rate.sleep();
       			 }
		}
};	





int main(int argc, char** argv){
        //Initialize ros system and create node object
        ros::init(argc, argv, "base_controller");
        ros::NodeHandle nh;

       
        // Create class instance
	BaseController base_controller = BaseController(&nh);
	
	// Run the main program
	base_controller.spin();
        
        return 0;
}
