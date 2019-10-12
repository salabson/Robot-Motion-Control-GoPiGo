#include<ros/ros.h>
#include<std_msgs/Float32.h>
#include<geometry_msgs/Twist.h>

void cmd_vel_callback(const geometry_msgs::Twist::ConstPtr& twist_msg){
        double vel_x = twist_msg->linear.x;
        double vel_th = twist_msg->angular.z;
        ROS_INFO("vel_x: %f angular_z: %f", vel_x, vel_th);
}
int main(int argc, char** argv){
        // Initialize ROS system and create node object
        ros::init(argc, argv,"diffdrive");
        ros::NodeHandle nh;

        // Create and initialize subscriber object
        ros::Subscriber vel_sub = nh.subscribe("/cmd_vel", 10, cmd_vel_callback);
        ros::spin();

}
