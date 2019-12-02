#include<ros/ros.h>
#include<std_msgs/Float32.h>

double robot_width = 0.14;

double lwheel_angular_vel_enc =0.0;
double rwheel_angular_vel_enc =0.0;

double lwheel_tangential_vel_enc=0.0;
double rwheel_tangential_vel_enc=0.0;

//intial position
double x =0.0;
double y =0.0;
double th =0.0;

// velocities
double v=0.0;
double w=0.0;


void lwheel_angular_vel_enc_callback(const std_msgs::Float32::ConstPtr &ang_vel_enc){
        ROS_INFO("lwheel vel %f:",ang_vel_enc->data);
        lwheel_angular_vel_enc=ang_vel_enc->data;

}

void rwheel_angular_vel_enc_callback(const std_msgs::Float32::ConstPtr &ang_vel_enc){
        ROS_INFO("rwheel vel: %f",ang_vel_enc->data);
        rwheel_angular_vel_enc=ang_vel_enc->data;
}

double compute_linear_velocity(){
        return (rwheel_tangential_vel_enc+lwheel_tangential_vel_enc)/2.0
}

double compute_rotational_velocity(){
        return (rwheel_tangential_vel_enc-lwheel_tangential_vel_enc)/robot_width
}
int main(int argc, char** argv){
        // initialize ros system
        ros::init(argc, argv, "diffdrive_odom");
        ros::NodeHandle nh;

        // Create and initialize subsciber objects
        ros::Subscriber lwheel_angular_vel_enc_sub = nh.subscribe("lwheel_angular_vel_enc", 10, 
                                                                  lwheel_angular_vel_enc_callback);
        ros::Subscriber rwheel_angular_vel_enc_sub = nh.subscribe("rwheel_angular_vel_enc", 10,
                                                                  rwheel_angular_vel_enc_callback);
                   
        ros::spin();

}
