#include<ros/ros.h>
#include<gopigo_interface/Encoder.h>
#include<std_msgs/Float32.h>
#include<math.h>

std_msgs::Float32 left_angular_vel_enc;
std_msgs::Float32 right_angular_vel_enc;


int N = 720; // ticks per revolution
double wheel_radius = 0.03;
double pi = M_PI;

double prev_left_ticks=0;
double prev_right_ticks=0;

double  delta_left_ticks=0;
double  delta_right_ticks=0;

double distance_left = 0;
double distance_right = 0;

double current_left_ticks =0;
double current_right_ticks =0;


double delta_time=0;




void encoder_callback(const gopigo_interface::Encoder::ConstPtr& enc_ticks){
 current_left_ticks = enc_ticks->left;
 current_right_ticks = enc_ticks->right;
 ROS_INFO("left tick:%f right tick:%f", current_left_ticks, current_right_ticks);
}


int main (int argc, char** argv){

// initialize ros system
ros::init(argc, argv, "robot_state_updater");
ros::NodeHandle nh;

// create a subscriber
ros::Subscriber encoder_sub = nh.subscribe("encoder_reading",10, encoder_callback);

// Create publisher objects
ros::Publisher left_angular_vel_enc_pub = nh.advertise<std_msgs::Float32>("left_angular_vel_enc",10);
ros::Publisher right_angular_vel_enc_pub = nh.advertise<std_msgs::Float32>("right_angular_vel_enc",10);

ros::Time current_timestamp = ros::Time::now();
ros::Time last_timestamp = ros::Time::now();

ros::Rate rate(20);
while(ros::ok()){
// calculate delta time
current_timestamp = ros::Time::now();
delta_time = (current_timestamp-last_timestamp).toSec();

// calculate diff between two encoder readings
delta_left_ticks = current_left_ticks - prev_left_ticks;
delta_right_ticks = current_right_ticks - prev_right_ticks;

// calculate distance travel by left and right wheels
distance_left = delta_left_ticks/N*2*pi*wheel_radius;
distance_right = delta_right_ticks/N*2*pi*wheel_radius;

// calculate encoder tangential velocities for both left and right wheels
double vl = distance_left/delta_time;
double vr = distance_right/delta_time;

// calculate encoder angular velocities for both left and right wheels
left_angular_vel_enc.data = vl/wheel_radius;
right_angular_vel_enc.data = vr/wheel_radius;

// publish encoder angular velocities 
left_angular_vel_enc_pub.publish(left_angular_vel_enc);
right_angular_vel_enc_pub.publish(right_angular_vel_enc);

// save current data for next iteration
last_timestamp = current_timestamp;
ros::spinOnce();
rate.sleep();

}

}
       
