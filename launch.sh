#!/bin/sh

xterm  -e  " gazebo " &
sleep 5
#xterm  -e  " source /opt/ros/kinetic/setup.bash; roscore" & 
xterm  -e  " source /opt/ros/noetic/setup.bash; roscore" & 
sleep 5
xterm  -e  " rosrun rviz rviz" 
