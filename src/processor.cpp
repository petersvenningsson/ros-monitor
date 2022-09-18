#include "processor.h"
#include "linux_parser.h"

#include <string>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>

using std::string;

#define CPU_LOAD_SAMPLE_TIME 1000 //ms

struct Core {
	long idle;
	long non_idle;
	float load;
};

void Processor::ParseLoad(string line, struct Core prev_core, struct Core& core)
{
	long s_user, s_nice, s_system, s_idle, s_iowait, s_irq, s_softirq;
	long s_steal, s_guest, s_guest_nice;
	long delta_idle, delta_non_idle;
	string s_name;

	std::istringstream linestream(line);
	if (linestream >> s_name  >> s_user >> s_nice >> s_system >>
		s_idle >> s_iowait >> s_irq >> s_softirq >> s_steal >>
		s_guest >> s_guest_nice) {
		
		core.idle = s_idle + s_iowait;
		core.non_idle = s_user + s_nice + s_system + s_irq + s_softirq + s_steal;	
		
		delta_idle = core.idle - prev_core.idle;
		delta_non_idle = core.non_idle - prev_core.non_idle;	
		
		core.load = delta_non_idle/(float)(delta_idle + delta_non_idle);
	}
}


void Processor::Reader(std::vector<float>& load_, std::mutex& data_guard_, std::atomic_bool& stop_token, int num_cores)
{
	string line;
	
	std::vector<struct Core> cores(num_cores+1, {0, 0, 0});
	std::vector<struct Core> prev_cores(num_cores+1, {0, 0, 0});
	
	while (!stop_token.load()) {
		std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
		for (int i=0; i<(num_cores+1); i++) {
	
			if (filestream.is_open() && std::getline(filestream, line)) {
				Processor::ParseLoad(line, prev_cores.at(i), cores.at(i));
				data_guard_.lock();
				load_.at(i) = cores.at(i).load;
				data_guard_.unlock();
			}

		prev_cores = cores;
		}	
	std::this_thread::sleep_for(std::chrono::milliseconds(CPU_LOAD_SAMPLE_TIME));
	}
}

std::vector<float> Processor::Utilization() {
	std::vector<float> local_load;

	data_guard_.lock();
	local_load = load_;
	data_guard_.unlock();

	return local_load;
}

Processor::Processor()
{
	num_cores_ = 4;
	load_.resize(num_cores_+1, 0.0);
	stop_token.store(false);
	reader_thread = std::thread(Reader,
			std::ref(load_),
			std::ref(data_guard_),
			std::ref(stop_token),
			num_cores_);
}

Processor::~Processor()
{
	stop_token.store(true);
	reader_thread.join();
}
