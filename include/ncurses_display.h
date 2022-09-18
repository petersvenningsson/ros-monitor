#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include "system.h"
#include "ros_monitor.h"

namespace NCursesDisplay {
void Display(System& system, RosMonitor& ros, int n = 10);
void DisplaySystem(System& system, WINDOW* window);
void DisplayRos(RosMonitor& ros, WINDOW* window);
std::string ProgressBar(float percent);
};

#endif
