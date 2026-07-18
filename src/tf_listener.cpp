// src/tf_listener.cpp
//
// Dynamic TF listener: at 10 Hz, looks up the pose of gripper_touchpoint
// relative to the world origin frame "odom" and prints (x, y, z).

#include <ros/ros.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "tf_listener");
    ros::NodeHandle n;

    tf2_ros::Buffer tf_buffer;
    tf2_ros::TransformListener tf_listener(tf_buffer);

    ros::Rate rate(10.0);  // 10 Hz

    const std::string target_frame = "odom";
    const std::string source_frame = "gripper_touchpoint";

    ROS_INFO("tf_listener: waiting for %s -> %s ...",
             target_frame.c_str(), source_frame.c_str());

    while (ros::ok())
    {
        try
        {
            geometry_msgs::TransformStamped transform =
                tf_buffer.lookupTransform(target_frame, source_frame,
                                           ros::Time(0), ros::Duration(1.0));

            double x = transform.transform.translation.x;
            double y = transform.transform.translation.y;
            double z = transform.transform.translation.z;

            ROS_INFO("gripper_touchpoint in odom -> x: %.3f  y: %.3f  z: %.3f",
                     x, y, z);
        }
        catch (tf2::TransformException& ex)
        {
            ROS_WARN_THROTTLE(5.0, "TF lookup failed: %s", ex.what());
        }

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
