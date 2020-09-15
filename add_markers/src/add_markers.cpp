#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Point.h>

#include <cmath>

nav_msgs::Odometry odom_global;
double dist_threshold = 0.3;

void odom_callback(nav_msgs::Odometry odom)
{
  odom_global = odom;
  //ROS_INFO("x:%.2f, y:%.2f ", (float)odom_global.pose.pose.position.x, (float)odom_global.pose.pose.position.y);
}

double euclid_distance(double x1, double x2, double y1, double y2)
{
  return sqrt( pow((x1 - x2), 2.0) + pow((y1 - y2), 2.0) );
}


int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(2);
  
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

  // Subscriber for /odom
  ros::Subscriber odom_sub = n.subscribe("/odom", 20, odom_callback);

  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;
  bool marker_picked = false;
  geometry_msgs::Point marker_pickup;
  geometry_msgs::Point marker_dropoff;

  // Define the pick up marker coordinates
  marker_pickup.x = 2.43;
  marker_pickup.y = -5.44;

  // Define the drop off marker coordinates
  marker_dropoff.x = 2.01;
  marker_dropoff.y = -0.301;

  while (ros::ok())
  {
    visualization_msgs::Marker marker;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "/map";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "add_markers";
    marker.id = 0;

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 0.25;
    marker.scale.y = 0.25;
    marker.scale.z = 0.25;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 0.0f;
    marker.color.b = 1.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();

    // Publish the marker
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }
    
    //ROS_INFO("Distance to 2:%.2f, \t Distance to 1:%.2f ", (float)euclid_distance(odom_global.pose.pose.position.x, marker_dropoff.x, odom_global.pose.pose.position.y, marker_dropoff.y), (float)euclid_distance(odom_global.pose.pose.position.x, marker_pickup.x, odom_global.pose.pose.position.y, marker_pickup.y));
    // Compute distance to drop zone. If it is less than threshold and it has been picked, place it down
    if(euclid_distance(odom_global.pose.pose.position.x, marker_dropoff.x, odom_global.pose.pose.position.y, marker_dropoff.y) < dist_threshold && marker_picked)
    {
      ROS_INFO("Marker droped");
      marker.action = visualization_msgs::Marker::ADD;
      marker.pose.position.x = marker_dropoff.x;
      marker.pose.position.y = marker_dropoff.y;
      // Publish marker
      marker_pub.publish(marker);
      //marker_picked = !marker_picked;
      //ros::Duration(5.0).sleep();
    }
    else   // marker has not been picked, draw at pick up location
    {
      //Compute distance to pick up zone. If it is less than threshold and it hasn't been picked, pick it up
      if(euclid_distance(odom_global.pose.pose.position.x, marker_pickup.x, odom_global.pose.pose.position.y, marker_pickup.y) < dist_threshold && !marker_picked)
      {
        ROS_INFO("Marker picked");
        marker.action = visualization_msgs::Marker::DELETE;
        // Publish marker
        marker_pub.publish(marker);
        marker_picked = !marker_picked;
      }
      else   // marker has not been picked, draw at pick up location
      {
        if(!marker_picked)
        {
          ROS_INFO("not  picked, marker at pick up");
          marker.pose.position.x = marker_pickup.x;
          marker.pose.position.y = marker_pickup.y;
          // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
          marker.action = visualization_msgs::Marker::ADD;
          // Publish marker
          marker_pub.publish(marker);
        }
      }
    }

    ros::spinOnce();
    r.sleep();
  }
  return 0;
}