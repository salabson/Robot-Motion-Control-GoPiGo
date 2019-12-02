#include<ros/ros.h>
#include<std_msgs/Float32.h>
#include<tf/transform_broadcaster.h>
#include<geometry_msgs/TransformStamped.h>
#include<geometry_msgs/Quaternion.h>
#include<nav_msgs/Odometry.h>
#include<math.h>

double robot_width = 0.14;
double robot_radius = 0.03;

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

//this function convert angular to tangential velocity 
double angular_2_tangential(double angular_vel){
        return angular_vel*robot_radius;
}

double compute_linear_velocity(){
        return (rwheel_tangential_vel_enc+lwheel_tangential_vel_enc)/2.0;
}

double compute_rotational_velocity(){
        return (rwheel_tangential_vel_enc-lwheel_tangential_vel_enc)/robot_width;
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

        ros::Publisher odom_pub = nh.advertise<nav_msgs::Odometry>("/odom",10);

        ros::Time current_timestamp = ros::Time::now();
        ros::Time last_timestamp = ros::Time::now();

        ros::Rate rate(20);
        while(ros::ok()){
            lwheel_tangential_vel_enc = angular_2_tangential(lwheel_angular_vel_enc);
            rwheel_tangential_vel_enc = angular_2_tangential(rwheel_angular_vel_enc);

            // compute linear  and angular velocities
            v = compute_linear_velocity();
            w = compute_rotational_velocity();

            current_timestamp = ros::Time::now();
            double dt = (current_timestamp - last_timestamp).toSec();
            
            //robot moving straight
            if(rwheel_tangential_vel_enc == lwheel_tangential_vel_enc){
                    // update robot position
                    x=x+v*dt*cos(th);
                    y=y+v*dt*sin(th);
            } 
            // robot moving in circle
            else{
                    // calculate instantenous center of curvature
                    double R = (robot_width/2.0)*(rwheel_tangential_vel_enc+lwheel_tangential_vel_enc)
                               /(rwheel_tangential_vel_enc-lwheel_angular_vel_enc);
                    
                    // update robot postion and orientation
                    x=x+(cos(w*dt)*R*sin(th)) - (sin(w*dt)*-R*cos(th)) + (x-R*sin(th));
                    y=y+(sin(w*dt)*R*sin(th)) + (cos(w*dt)*-R*cos(th)) + (y+R*cos(th));
                    th=th+w*dt;
            }
            ROS_INFO("x: %f y: %f th: %f v: %f w: %f", x, y,th,v,w);

            // set up TF
            tf::TransformBroadcaster broadcaster;
            geometry_msgs::TransformStamped odom_trans;
            odom_trans.header.stamp = current_timestamp;
            odom_trans.header.frame_id ="odom";
            odom_trans.child_frame_id ="base_footprint";

            //update tf
            odom_trans.transform.translation.x = x;
            odom_trans.transform.translation.y = y;
            odom_trans.transform.translation.z = 0.0;
            odom_trans.transform.rotation =  tf::createQuaternionMsgFromYaw(th);


            // set up odom
            geometry_msgs::Quaternion odom_quat;
            odom_quat = tf::createQuaternionMsgFromRollPitchYaw(0.0,0.0,th);
            nav_msgs::Odometry odom;
            odom.header.stamp = current_timestamp;
            odom.header.frame_id="odom";
            odom.child_frame_id="base_footprint";

            //update odom position
            odom.pose.pose.position.x = x;
            odom.pose.pose.position.y = y;
            odom.pose.pose.position.z = 0.0;
            odom.pose.pose.orientation = odom_quat;

            // update odom velocity
            odom.twist.twist.linear.x = v;
            odom.twist.twist.linear.y = 0.0;
            odom.twist.twist.linear.z = 0.0;
            odom.twist.twist.angular.x = 0.0;
            odom.twist.twist.angular.y = 0.0;
            odom.twist.twist.angular.z = w;

            // publish odom and tf data
            odom_pub.publish(odom);
            broadcaster.sendTransform(odom_trans);

            // save current time for next iteration
            last_timestamp = current_timestamp;

            ros::spinOnce();
            rate.sleep();

        }
        return 0;
}

