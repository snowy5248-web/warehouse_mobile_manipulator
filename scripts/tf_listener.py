#!/usr/bin/env python3
# scripts/tf_listener.py
#
# Dynamic TF listener: at 10 Hz, looks up the pose of gripper_touchpoint
# relative to the world origin frame "odom" and prints (x, y, z).
import rospy
import tf2_ros


def main():
    rospy.init_node('tf_listener', anonymous=True)
    tf_buffer = tf2_ros.Buffer()
    tf_listener = tf2_ros.TransformListener(tf_buffer)
    rate = rospy.Rate(10.0)  # 10 Hz
    target_frame = 'odom'
    source_frame = 'gripper_touchpoint'
    rospy.loginfo("tf_listener: waiting for %s -> %s ...", target_frame, source_frame)
    while not rospy.is_shutdown():
        try:
            transform = tf_buffer.lookup_transform(
                target_frame, source_frame, rospy.Time(0), rospy.Duration(1.0)
            )
            t = transform.transform.translation
            rospy.loginfo(
                "gripper_touchpoint in odom -> x: %.3f  y: %.3f  z: %.3f",
                t.x, t.y, t.z
            )
        except (tf2_ros.LookupException,
                tf2_ros.ConnectivityException,
                tf2_ros.ExtrapolationException) as ex:
            rospy.logwarn_throttle(5.0, "TF lookup failed: %s", str(ex))
        rate.sleep()


if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass

