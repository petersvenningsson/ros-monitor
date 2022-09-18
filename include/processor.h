#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>

class Processor {
	public:
		Processor();
		~Processor();
		std::vector<float> Utilization();
		int NumCores() {return num_cores_;}
		static void Reader(std::vector<float>&, std::mutex&, std::atomic_bool&, int);
		static void ParseLoad(std::string, struct Core, struct Core&);
		Processor(const Processor&) = delete;
		Processor& operator=(const Processor&) = delete;
	private:
		std::thread reader_thread;
		std::atomic_bool stop_token;
		std::mutex data_guard_; 
		std::vector<float> load_;
		int num_cores_;
};

#endif
