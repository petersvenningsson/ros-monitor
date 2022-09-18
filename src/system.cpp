#include "system.h"
#include "processor.h"

System::System() : cpu_()
{
	os_ = LinuxParser::OperatingSystem();
	kernel_ = LinuxParser::Kernel();
}

Processor& System::Cpu() { return cpu_; }

std::string System::Kernel() { return kernel_; }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return os_; }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime()
{ 	
	return LinuxParser::UpTime();
}
