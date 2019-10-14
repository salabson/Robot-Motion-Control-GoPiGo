#include<ros/ros.h>
#include<std_msgs/Float32.h>
#include<geometry_msgs/Twist.h>

double robot_width;

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
}
int main(int argc, char** argv){
        // Initialize ROS system and create node object
        ros::init(argc, argv,"diffdrive");
        ros::NodeHandle nh;

        // Create and initialize subscriber object
        ros::Subscriber vel_sub = nh.subscribe("/cmd_vel", 10, cmd_vel_callback);
        ros::spin();

}
