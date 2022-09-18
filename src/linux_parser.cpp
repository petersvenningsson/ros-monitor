#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <limits>
#include <cmath>
#include <thread>
#include <chrono>
#include <iostream>
#include <atomic>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
	string line;
	string key;
	string value;
	std::ifstream filestream(kOSPath);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::replace(line.begin(), line.end(), ' ', '_');
 			std::replace(line.begin(), line.end(), '=', ' ');
			std::replace(line.begin(), line.end(), '"', ' ');
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "PRETTY_NAME") {
					std::replace(value.begin(), value.end(), '_', ' ');
					return value;
				}
 			}
		}
	}
	return value;
}

string LinuxParser::Kernel() {
	string os, kernel, version;
	string line;
	std::ifstream stream(kProcDirectory + kVersionFilename);
	if (stream.is_open()) {
 		std::getline(stream, line);
		std::istringstream linestream(line);
		linestream >> os >> version >> kernel;
	}
	return kernel;
}

float LinuxParser::MemoryUtilization()
{
	string line, key;
	float value;
	float mem_total = std::numeric_limits<double>::quiet_NaN();
	float mem_free = std::numeric_limits<double>::quiet_NaN();

	std::ifstream filestream(kProcDirectory + kMeminfoFilename);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::istringstream linestream(line);
			if (linestream >> key >> value) {
				if (key == "MemTotal:") {
					mem_total = value;
				} else if (key  == "MemFree:") {
					mem_free = value;
				}
			}
		}
	}
	if (std::isnan(mem_free) || std::isnan(mem_total)) 
		return 0.0;
	else
		return (mem_total - mem_free) / mem_total;
}

long LinuxParser::UpTime()
{
	string line;
	long uptime;
	std::ifstream filestream(kProcDirectory + kUptimeFilename);
	if (filestream.is_open() && std::getline(filestream, line)) {
		std::istringstream linestream(line);
		if (linestream >> uptime)
			return uptime;
	}
	return 0;
}

int LinuxParser::TotalProcesses()
{
	string line, key;
	float value;

	std::ifstream filestream(kProcDirectory + kStatFilename);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::istringstream linestream(line);
			if (linestream >> key >> value) 
				if (key == "processes") 
					 return value;
		}
	}
	return 0;
}