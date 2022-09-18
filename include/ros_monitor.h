#ifndef ROS_MONITOR_H
#define ROS_MONITOR_H

#include <vector>
#include <string>
#include <ros/ros.h>

struct Topic : ros::master::TopicInfo {
	double hz;
};

class RosMonitor {
	public:
		RosMonitor();
		std::vector<Topic> Topics();
	private:
		std::vector<Topic> topics_;
		std::string uri_;
};
#endif
