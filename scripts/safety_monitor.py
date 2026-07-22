#!/usr/bin/env python3
# scripts/safety_monitor.py
#
# Safety monitor node: listens to /scan (sensor_msgs/LaserScan),
# finds the distance to the nearest obstacle, and prints a large
# warning to the terminal if that distance is below 0.3 m.
import rospy
from sensor_msgs.msg import LaserScan

SAFE_DISTANCE = 0.3  # meters


def scan_callback(msg):
    # Filter out invalid readings (0.0, inf, nan) before taking the min
    valid_ranges = [
        r for r in msg.ranges
        if r > msg.range_min and r < msg.range_max
    ]

    if not valid_ranges:
        return

    min_distance = min(valid_ranges)

    if min_distance < SAFE_DISTANCE:
        rospy.logwarn(
            "\n"
            "##########################################################\n"
            "#   WARNING: TOO CLOSE TO OBSTACLE (< 0.3m)             #\n"
            "#   Nearest obstacle distance: %.3f m                    #\n"
            "##########################################################",
            min_distance
        )
    else:
        rospy.loginfo_throttle(
            2.0, "safety_monitor: nearest obstacle at %.3f m", min_distance
        )


def main():
    rospy.init_node('safety_monitor_node', anonymous=True)
    rospy.Subscriber('/scan', LaserScan, scan_callback)
    rospy.loginfo("safety_monitor_node: monitoring /scan for obstacles closer than %.2f m", SAFE_DISTANCE)
    rospy.spin()


if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
