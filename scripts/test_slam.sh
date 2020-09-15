#!/bin/sh
# Launch  turtlebot in your environment
xterm  -e  " roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$(rospack find map)/world/MyAppartment.world" &
sleep 5
Perform SLAM
xterm  -e  " roslaunch turtlebot_gazebo gmapping_demo.launch " &
sleep 5
# Observe the map in rviz
xterm  -e  " roslaunch turtlebot_rviz_launchers view_navigation.launch " &
sleep 
# Manually control the robot with keyboard commands
xterm  -e  " roslaunch turtlebot_teleop keyboard_teleop.launch " &
sleep 5
