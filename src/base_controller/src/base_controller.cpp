#include<ros/ros.h>
#include<std_msgs/Float32.h>
#include<math.h>

std_msgs::Float32 rwheel_motorcmd;
std_msgs::Float32 lwheel_motorcmd;

double rwheel_tangential_vel;
double lwheel_tangential_vel;

double wheel_radius = 0.03;

double max_ang_vel = 5.0;
double min_ang_vel = 1.90;
double max_motor_cmd = 100;
double min_motor_cmd = 43;



// right wheel tangential velocity callback
void rwheel_tang_vel_callback(const std_msgs::Float32::ConstPtr& rtang_vel){
        rwheel_tangential_vel = rtang_vel->data;
	ROS_INFO("rtan: %f",rwheel_tangential_vel );
    
}

// left wheel tangential velocity callback
void lwheel_tang_vel_callback(const std_msgs::Float32::ConstPtr& ltang_vel){
        lwheel_tangential_vel= ltang_vel->data;
	ROS_INFO("ltan: %f",lwheel_tangential_vel );

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



// Compute  motor command
double  calculate_motorcmd(double tang_vel){
        double angular_vel = tangential_2_angular_vel(tang_vel);
        return angular_2_motor_cmd(angular_vel);
        
}

int main(int argc, char** argv){
        //Initialize ros system and create node object
        ros::init(argc, argv, "base_controller");
        ros::NodeHandle nh;

        // Create subscribers objects
        ros::Subscriber rwheel_tang_vel_sub = nh.subscribe("/rwheel_tangential_vel",10,rwheel_tang_vel_callback); 
        ros::Subscriber lwheel_tang_vel_sub = nh.subscribe("/lwheel_tangential_vel",10,lwheel_tang_vel_callback);

        // Create publishers objects
        ros::Publisher rwheel_motorcmd_pub = nh.advertise<std_msgs::Float32>("/rwheel_motorcmd",10);  
        ros::Publisher lwheel_motorcmd_pub = nh.advertise<std_msgs::Float32>("/lwheel_motorcmd",10); 

        ros::Rate rate(20);

        //start publishing
        while(ros::ok()){ 
                ROS_INFO("rtan: %f ltan: %f", rwheel_tangential_vel, lwheel_tangential_vel);
                rwheel_motorcmd.data = calculate_motorcmd(rwheel_tangential_vel);
                lwheel_motorcmd.data = calculate_motorcmd(lwheel_tangential_vel);

                rwheel_motorcmd_pub.publish(rwheel_motorcmd);
                lwheel_motorcmd_pub.publish(lwheel_motorcmd);
                ros::spinOnce();
                rate.sleep();
        }
        
        return 0;
}
