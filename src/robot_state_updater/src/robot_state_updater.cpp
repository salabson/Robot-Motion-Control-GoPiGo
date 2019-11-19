#include<ros/ros.h>
#include<gopigo_interface/Encoder.h>

void encoder_callback(const gopigo_interface::Encoder::ConstPtr& enc_ticks){
 double left_ticks = enc_ticks->left;
 double right_ticks = enc_ticks->right;
 ROS_INFO("left tick:%f right tick:%f", left_ticks, right_ticks);
}


int main (int argc, char** argv){

// initialize ros system
ros::init(argc, argv, "robot_state_updater");
ros::NodeHandle nh;

// create a subscriber
ros::Subscriber encoder_sub = nh.subscribe("encoder_reading",10, encoder_callback);

// let ros take over
ros::spin();

}
       
