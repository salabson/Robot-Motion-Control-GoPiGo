#include<ros/ros.h>
#include<std_msgs/Float32.h>

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
            // save current time for next iteration
            last_timestamp = current_timestamp;

        }
}

