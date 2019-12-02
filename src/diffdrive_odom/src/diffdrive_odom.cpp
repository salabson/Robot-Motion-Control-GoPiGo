#include<ros/ros.h>
#include<std_msgs/Float32.h>

void lwheel_angular_vel_enc_callback(const std_msgs::Float32::ConstPtr &ang_vel_enc){
        ROS_INFO("lwheel vel %f:",ang_vel_enc->data);

}

void rwheel_angular_vel_enc_callback(const std_msgs::Float32::ConstPtr &ang_vel_enc){
        ROS_INFO("rwheel vel: %f",ang_vel_enc->data);
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
