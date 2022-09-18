#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

class System {
	public:
		System();
		Processor& Cpu();
		float MemoryUtilization();
		long UpTime();
		int TotalProcesses();
		std::string Kernel();
		std::string OperatingSystem();

	private:
		Processor cpu_;
		std::string os_;
		std::string kernel_;
};

#endif
