#include "format.h"

#include <string>
#include <sstream>

using std::string;

string Format::ElapsedTime(long seconds) {
	int minutes = seconds/60;
	int hours = minutes/60;
	seconds = seconds % 60;
	minutes = minutes % 60;

	std::ostringstream ss;
	ss << hours << "h: " << minutes << "m: " << seconds << "s";
	string str = ss.str();
	return str;
}