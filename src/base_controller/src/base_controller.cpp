#include<ros/ros.h>
#include<std_msgs/Float32.h>

double rwheel_tangential_vel;
double lwheel_tangential_vel;

double wheel_radius;

double tangential_2_angular_vel(double tang_vel){
        return tang_vel/wheel_radius;

}

void rwheel_tang_vel_callback(const std_msgs::Float32::ConstPtr& rtang_vel){
        rwheel_tangential_vel = rtang_vel->data;
	ROS_INFO("rtan: %f",rwheel_tangential_vel );
    
}

void lwheel_tang_vel_callback(const std_msgs::Float32::ConstPtr& ltang_vel){
        lwheel_tangential_vel= ltang_vel->data;
	ROS_INFO("ltan: %f",lwheel_tangential_vel );

}

int main(int argc, char** argv){
        //Initialize ros system and create node object
        ros::init(argc, argv, "base_controller");
        ros::NodeHandle nh;

        // Create subscribers objects
        ros::Subscriber rwheel_tang_vel_sub = nh.subscribe("/rwheel_tangential_vel",10,rwheel_tang_vel_callback); 
        ros::Subscriber lwheel_tang_vel_sub = nh.subscribe("/lwheel_tangential_vel",10,lwheel_tang_vel_callback);
        ros::spin();
}
