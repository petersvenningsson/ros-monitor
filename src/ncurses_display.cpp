#include "ncurses_display.h"

#include <curses.h>
#include <chrono>
#include <string>
#include <thread>
#include <vector>

#include "format.h"
#include "system.h"

using std::string;
using std::to_string;

std::string NCursesDisplay::ProgressBar(float percent) {
	std::string result{};
	int size{50};
	float bars{percent * size};

	for (int i{0}; i < size; ++i) {
		result += i <= bars ? '|' : ' ';
	}

	string display{to_string(percent * 100).substr(0, 4)};
	if (percent < 0.1 || percent == 1.0)
		display = " " + to_string(percent * 100).substr(0, 3);
	return result + " " + display + "/100%";
}

void NCursesDisplay::DisplaySystem(System& system, WINDOW* window) {
	int row{0};
	mvwprintw(window, ++row, 2, ("OS: " + system.OperatingSystem()).c_str());
	mvwprintw(window, ++row, 2, ("Kernel: " + system.Kernel()).c_str());
	for (int i=0; i<(system.Cpu().NumCores()+1); i++) {
		string cpu_entry = (i) 
			? ("CPU" + std::to_string(i) + ": ") : "CPU: ";
		mvwprintw(window, ++row, 2, cpu_entry.c_str());
		wattron(window, COLOR_PAIR(1));
		mvwprintw(window, row, 10, "");
		wprintw(window, ProgressBar(
			system.Cpu().Utilization().at(i)).c_str());
		wattroff(window, COLOR_PAIR(1));
	}
	mvwprintw(window, ++row, 2, "Memory: ");
	wattron(window, COLOR_PAIR(1));
	mvwprintw(window, row, 10, "");
	wprintw(window, ProgressBar(system.MemoryUtilization()).c_str());
	wattroff(window, COLOR_PAIR(1));
	mvwprintw(window, ++row, 2, (
		"Total Processes: " + to_string(system.TotalProcesses())).c_str());
	mvwprintw(window, ++row, 2, (
		"Up Time: " + Format::ElapsedTime(system.UpTime())).c_str());
	wrefresh(window);
}

void NCursesDisplay::DisplayRos(RosMonitor& ros, WINDOW* window) {
	int row{0};
	int const topic_column{2};
	int const datatype_column{25};
	int const hz_column{46};
	wattron(window, COLOR_PAIR(2));
	mvwprintw(window, ++row, topic_column, "topic");
	mvwprintw(window, row, datatype_column, "message type");
	mvwprintw(window, row, hz_column, "Hz");
	wattroff(window, COLOR_PAIR(2));
	for (auto topic: ros.Topics()) {
		mvwprintw(window, ++row, topic_column, topic.name.c_str());
		mvwprintw(window, row, datatype_column, topic.datatype.c_str());
	}
}

void NCursesDisplay::Display(System& system, RosMonitor& ros_monitor, int n) {
	initscr();
	noecho();
	cbreak();
	start_color();

	int x_max{getmaxx(stdscr)};
	WINDOW* system_window = newwin(13, x_max - 1, 0, 0);
	WINDOW* ros_window =
		newwin(3 + n, x_max - 1, system_window->_maxy + 1, 0);

	while (1) {
		init_pair(1, COLOR_BLUE, COLOR_BLACK);
		init_pair(2, COLOR_GREEN, COLOR_BLACK);
		box(system_window, 0, 0);
		box(ros_window, 0, 0);
		DisplaySystem(system, system_window);
		DisplayRos(ros_monitor, ros_window);
		wrefresh(system_window);
		wrefresh(ros_window);
		refresh();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	endwin();
}
