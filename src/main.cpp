#include "ncurses_display.h"
#include "system.h"
#include "processor.h"
#include "ros_monitor.h"

#include <chrono>
#include <iostream>
#include <string>
#include "ros/ros.h"

int main(int argc, char **argv) {
	ros::init(argc, argv, "monitor");

	RosMonitor ros_monitor;
	System system;
	NCursesDisplay::Display(system, ros_monitor, 4);
}
