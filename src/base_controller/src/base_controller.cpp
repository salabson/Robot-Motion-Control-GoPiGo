#include<ros/ros.h>
#include<std_msgs/Float32.h>

std_msgs::Float32 rwheel_tangential_vel;
std_msgs::Float32 lwheel_tangential_vel;

void rwheel_tang_vel_callback(const std_msgs::Float32::ConstPtr& rtang_vel){
        rwheel_tangential_vel.data = rtang_vel->data;
    
}

void lwheel_tang_vel_callback(const std_msgs::Float32::ConstPtr& ltang_vel){
        lwheel_tangential_vel.data = ltang_vel->data;

}

int main(int argc, char** argv){
        //Initialize ros system and create node object
        ros::init(argc, argv, "base_controller");
        ros::NodeHandle nh;

        // Create subscribers objects
        ros::Subscriber rwheel_tang_vel_sub = nh.subscribe("/rwheel_tangential_vel",10,rwheel_tang_vel_callback); 
        ros::Subscriber lwheel_tang_vel_sub = nh.subscribe("/lwheel_tangential_vel",10,lwheel_tang_vel_callback);
        ROS_INFO("rtan: %f ltan: %f");
        ros::spin();
}
