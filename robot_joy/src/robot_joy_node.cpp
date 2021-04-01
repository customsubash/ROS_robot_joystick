#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>

// create the TeleopRobot class and define the joyCallback function that will take a joy msg
class TeleopRobot
{
public:
  TeleopRobot();

private:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
  ros::NodeHandle nh_;

  int linear_, angular_;   // used to define which axes of the joystick will control our robot
  //char topic_name[20]="cmd_vel";
  double l_scale_, a_scale_;
  ros::Publisher vel_pub_;
  ros::Subscriber joy_sub_;

};

TeleopRobot::TeleopRobot(): linear_(1), angular_(2)
{
  //  initialize some parameters
  nh_.param("axis_linear", linear_, linear_);  
  nh_.param("axis_angular", angular_, angular_);
  nh_.param("scale_angular", a_scale_, a_scale_);
  nh_.param("scale_linear", l_scale_, l_scale_);
  //nh_.param("topic_name", topic_name, topic_name)
  // create a publisher that will advertise on the command_velocity topic of the robot
  vel_pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 1);

  // subscribe to the joystick topic for the input to drive the robot
  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &TeleopRobot::joyCallback, this);
}


void TeleopRobot::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  geometry_msgs::Twist twist;

  // take the data from the joystick and manipulate it by scaling it and using independent axes to control the linear and angular velocities of the robot
  twist.angular.z = a_scale_*joy->axes[angular_];
  twist.linear.x = l_scale_*joy->axes[linear_];

  vel_pub_.publish(twist); 
}
int main(int argc, char** argv)
{
  // initialize our ROS node, create a teleop_robot, and spin our node until Ctrl-C is pressed
  ros::init(argc, argv, "teleop_robot");
  TeleopRobot teleop_robot;

  ros::spin();
}
