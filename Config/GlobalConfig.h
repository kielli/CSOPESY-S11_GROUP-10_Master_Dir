#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <iomanip> // For quoted string parsing
#include <random>

#include "../TypedefRepo.h"

class GlobalConfig
{
public:
	static GlobalConfig* getInstance() {
		if (sharedInstance == nullptr) {
			sharedInstance = new GlobalConfig();
		}
		return sharedInstance;
	}

	static void initialize();
	static void destroy();
	void loadConfigFile(String& filename);				// Loads the config file
	void printConfig() const;							// Prints the config

	// Getters
	int getNumCPU() const;							// Returns the number of CPUs
	String getScheduler() const;						// Returns the scheduler
	int getQuantumCycles() const;					// Returns the quantum cycles
	int getBatchProcessFreq() const;				// Returns the batch process frequency
	//int getMinIns() const;							// Returns the minimum instructions
	//int getMaxIns() const;							// Returns the maximum instructions
	int getDelaysPerExec() const;					// Returns the delays per execution
	int getMaxOverallMem() const;					// Returns the maximum overall memory
	int getMemPerFrame() const;						// Returns the memory per frame
	
	int getTotalInstructionsPerProcess() const;						// Returns the total lines
	int getTotalMemoryPerProcess() const;					// Returns the memory per process

private:
	GlobalConfig();
	static GlobalConfig* sharedInstance;

	struct Config {							// Struct variable to hold the values of the config.txt
		int num_cpu;					// Number of CPUs: [1, 128]
		String scheduler;					// or enum SchedulerType { FCFS, RR };
		int quantum_cycles;			// For Round Robin: [1, 2^32]
		int batch_process_freq;		// Frequency of batch process: [1, 2^32]
		int min_ins;					// Minimum instructions: [1, 2^32]
		int max_ins;					// Maximum instructions: [1, 2^32]
		int delays_per_exec;			// Delays per execution: [0, 2^32]
		int max_overall_mem;			// Maximum overall memory: [1, 2^32]
		int mem_per_frame;				// Memory per frame: [1, 2^32]; Also, memory size of each process
		int min_mem_per_process;		// Minimum memory per process: [1, 2^32]
		int max_mem_per_process;		// Maximum memory per process: [1, 2^32]
	};

	Config config;										// Config variable
	bool parseConfigFile(String& line);					// Parses the config file

	int generateRandomInstructionCount() const;				// Returns a random instruction count
	int generateRandomMemoryCount() const;					// Returns a random memory count
};

