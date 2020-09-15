#!/bin/sh
# Launch  turtlebot in your environment
xterm  -e  " roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$(rospack find map)/world/MyAppartment.world " &
sleep 5
# Localize the turtlebot
xterm  -e  " roslaunch turtlebot_gazebo amcl_demo.launch map_file:=$(rospack find map)/map/gmapping.yaml " &
sleep 5 
# Observe the map in rviz
xterm  -e  " roslaunch rvizConfig rvizConfig.launch " &
sleep 10 
# Launch add_markers
xterm  -e  " roslaunch add_markers add_markers.launch " &
