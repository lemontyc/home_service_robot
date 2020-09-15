#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  // Initialize the pick_objects node
  ros::init(argc, argv, "pick_objects");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal first_goal;
  move_base_msgs::MoveBaseGoal second_goal;

  // set up the frame parameters
  first_goal.target_pose.header.frame_id = "map";
  first_goal.target_pose.header.stamp = ros::Time::now();

  second_goal.target_pose.header.frame_id = "map";
  second_goal.target_pose.header.stamp = ros::Time::now();

  // Define a position and orientation for the robot's first goal
  first_goal.target_pose.pose.position.x    = 2.43;
  first_goal.target_pose.pose.position.y    = -5.44;
  first_goal.target_pose.pose.orientation.w = 0.0025;

  // Define a position and orientation for the robot's second goal
  second_goal.target_pose.pose.position.x    = 2.01;
  second_goal.target_pose.pose.position.y    = -0.301;
  second_goal.target_pose.pose.orientation.w = -0.00534;

  // Send the pickup zone position and orientation for the robot to reach
  ROS_INFO("Robot is travelling to the pickup zone");

  ac.sendGoal(first_goal);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Robot picked up the virtual object");
  else
    ROS_INFO("Robot failed to move, for some reason");

  ros::Duration(5.0).sleep();

  // Send the drop off zone position and orientation for the robot to reach
  ROS_INFO("Robot is travelling to the drop off zone");
  
  ac.sendGoal(second_goal);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Robot dropped the virtual object");
  else
    ROS_INFO("Robot failed to move, for some reason");

  


  return 0;
}