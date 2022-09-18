
#include "ros_monitor.h"

RosMonitor::RosMonitor()
{
	std::string uri_ = ros::master::getURI();
	Topic topic;
	ros::master::V_TopicInfo v_topics;
	
	ros::master::getTopics(v_topics);
	for (auto v_topic: v_topics) {
		topic.name = v_topic.name;
		topic.datatype = v_topic.datatype;
		topics_.push_back(topic);
	}
}

std::vector<Topic> RosMonitor::Topics()
{
	return topics_;
}
