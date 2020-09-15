#!/bin/sh
# Launch  turtlebot in your environment
xterm  -e  " roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$(rospack find map)/world/MyAppartment.world " &
sleep 10
# Localize the turtlebot
xterm  -e  " roslaunch turtlebot_gazebo amcl_demo.launch map_file:=$(rospack find map)/map/gmapping.yaml " &
sleep 5 
# Observe the map in rviz
xterm  -e  " roslaunch rvizConfig rvizConfig.launch " &
sleep 5
# Launch add_markers
xterm  -e  " roslaunch add_markers add_markers.launch " &
sleep 5
# Launch add_markers
xterm  -e  " roslaunch pick_objects pick_objects.launch " &
