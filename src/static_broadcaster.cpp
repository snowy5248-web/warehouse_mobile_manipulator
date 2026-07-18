// Publishes the fixed transform base_link -> laser_frame.
// The offset MUST match the <pose> given to the lidar <sensor> tag
// in urdf/robot.urdf (0.15 m forward, 0.15 m up), otherwise the
// laser scan visualized in RViz will not line up with the robot model.

#include <ros/ros.h>
#include <tf2_ros/static_transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "static_broadcaster");
    ros::NodeHandle n;

    ROS_INFO("**** static_broadcaster: base_link -> laser_frame ****");

    static tf2_ros::StaticTransformBroadcaster static_broadcaster;
    geometry_msgs::TransformStamped static_transform;

    static_transform.header.stamp = ros::Time::now();
    static_transform.header.frame_id = "base_link";
    static_transform.child_frame_id  = "laser_frame";

    // 15 cm in front of the chassis center, 5 cm above the top surface
    // (top surface sits at z = 0.10 relative to base_link -> 0.10 + 0.05 = 0.15)
    static_transform.transform.translation.x = 0.15;
    static_transform.transform.translation.y = 0.0;
    static_transform.transform.translation.z = 0.15;

    static_transform.transform.rotation.x = 0.0;
    static_transform.transform.rotation.y = 0.0;
    static_transform.transform.rotation.z = 0.0;
    static_transform.transform.rotation.w = 1.0;

    static_broadcaster.sendTransform(static_transform);

    // Static transforms are latched, but we still need to keep the node
    // alive so the transform stays available on /tf_static.
    ros::spin();

    return 0;
}
