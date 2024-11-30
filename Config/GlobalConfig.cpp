#include "GlobalConfig.h"

using namespace std;

GlobalConfig* GlobalConfig::sharedInstance = nullptr;

GlobalConfig::GlobalConfig() {
	config.num_cpu = 0;
	config.scheduler = "";
	config.quantum_cycles = 0;
	config.batch_process_freq = 0;
	config.min_ins = 0;
	config.max_ins = 0;
	config.delays_per_exec = 0;
	config.max_overall_mem = 0;
	config.mem_per_frame = 0;
	config.min_mem_per_process = 0;
	config.max_mem_per_process = 0;
}

void GlobalConfig::initialize()
{
	if (sharedInstance == nullptr) {
		sharedInstance = new GlobalConfig();
	}
}

void GlobalConfig::destroy()
{
	if (sharedInstance != nullptr) {
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

void GlobalConfig::loadConfigFile(String& filename)
{
	try {
		ifstream file(filename);
		if (!file.is_open()) {
			throw runtime_error("Error: Could not open file " + filename);
		}
		else {
			String line;
			while (getline(file, line)) {
				if (!parseConfigFile(line)) {
					throw runtime_error("Error: Could not parse line " + line);
					file.close();
					break;
				}
			}

			file.close();
		}
	}
	catch (const runtime_error& e) {
		cerr << e.what() << endl;
	}
}

bool GlobalConfig::parseConfigFile(String& line) {
	bool isParsed = false;

	istringstream iss(line);
	String key;

	if (!(iss >> key)) {
		isParsed = false;		// Empty line
	}

	try {
		if (key == "num-cpu") {
			int value;
			if (iss >> value && value >= 1 && value <= 128) {
				config.num_cpu = static_cast<uint8_t>(value);
				isParsed = true;
			}
			else {
				throw invalid_argument("num-cpu must be between 1 and 128");
			}
		}
		else if (key == "scheduler") {
			String value;
			iss >> quoted(value); // Handles quoted strings
			if (value == "rr" || value == "fcfs") {
				config.scheduler = value;
				isParsed = true;
			}
			else {
				throw invalid_argument("scheduler must be \"rr\" or \"fcfs\"");
			}
		}
		else if (key == "quantum-cycles") {
			uint32_t value;
			if (iss >> value && value >= 1) {
				config.quantum_cycles = value;
				isParsed = true;
			}
			else {
				throw invalid_argument("quantum-cycles must be >= 1");
			}
		}
		else if (key == "batch-process-freq") {
			uint32_t value;
			if (iss >> value && value >= 1) {
				config.batch_process_freq = value;
				isParsed = true;
			}
			else {
				throw invalid_argument("batch-process-freq must be >= 1");
			}
		}
		else if (key == "min-ins") {
			uint32_t value;
			if (iss >> value && value >= 1) {
				config.min_ins = value;
				isParsed = true;
			}
			else {
				throw invalid_argument("min-ins must be >= 1");
			}
		}
		else if (key == "max-ins") {
			uint32_t value;
			if (iss >> value && value >= config.min_ins) {
				config.max_ins = value;
				isParsed = true;
			}
			else {
				throw invalid_argument("max-ins must be >= min-ins");
			}
		}
		else if (key == "delay-per-exec") {
			uint32_t value;
			if (iss >> value) {
				config.delays_per_exec = value;
				isParsed = true;
			}
			else {
				throw invalid_argument("Invalid delay-per-exec value");
			}
		}
		else if (key == "max-overall-mem") {
			uint32_t value;
			if (iss >> value) {
				config.max_overall_mem = value;
				isParsed = true;
			}
			else {
				throw invalid_argument("Invalid max-overall-mem value");
			}
		}
		else if (key == "mem-per-frame") {
			uint32_t value;
			if (iss >> value) {
				config.mem_per_frame = value;
				isParsed = true;
			}
			else {
				throw invalid_argument("Invalid mem-per-frame value");
			}
		}
		else if (key == "min-mem-per-proc") {
			uint32_t value;
			if (iss >> value) {
				config.min_mem_per_process = value;
				isParsed = true;
			}
			else {
				throw invalid_argument("Invalid mem-per-process value");
			}
		}
		else if (key == "max-mem-per-proc") {
			uint32_t value;
			if (iss >> value) {
				config.max_mem_per_process = value;
				isParsed = true;
			}
			else {
				throw invalid_argument("Invalid mem-per-process value");
			}
		}
		else {
			throw invalid_argument("Unknown configuration key: " + key);
		}
	}
	catch (const invalid_argument& e) {
		cerr << "Error: " << e.what() << " in line: " << line << endl;
		isParsed = false;
	}


	return isParsed;
}

void GlobalConfig::printConfig() const
{
	cout << "Number of CPUs: " << static_cast<int>(config.num_cpu) << endl;
	cout << "Scheduler: " << config.scheduler << endl;
	cout << "Quantum Cycles: " << config.quantum_cycles << endl;
	cout << "Batch Process Frequency: " << config.batch_process_freq << endl;
	cout << "Min Instructions: " << config.min_ins << endl;
	cout << "Max Instructions: " << config.max_ins << endl;
	cout << "Delays per Execution: " << config.delays_per_exec << endl;
	cout << "Max Overall Memory: " << config.max_overall_mem << endl;
	cout << "Memory per Frame: " << config.mem_per_frame << endl;
	cout << "Minimum memory per Process: " << config.min_mem_per_process << endl;
	cout << "Maximum memory per Process: " << config.max_mem_per_process << endl;
}

int GlobalConfig::getRandomInstructionCount() const
{
	int linesOfCode = 0;

	if (config.min_ins == config.max_ins) {
		linesOfCode = config.min_ins;
	}
	else {
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dis(config.min_ins, config.max_ins);
		linesOfCode = dis(gen);
	}

	return linesOfCode;
}

uint16_t GlobalConfig::getNumCPU() const {
	return config.num_cpu;
}

String GlobalConfig::getScheduler() const {
	return config.scheduler;
}

uint32_t GlobalConfig::getQuantumCycles() const {
	return config.quantum_cycles;
}

uint32_t GlobalConfig::getBatchProcessFreq() const {
	return config.batch_process_freq;
}

uint32_t GlobalConfig::getMinIns() const {
	return config.min_ins;
}

uint32_t GlobalConfig::getMaxIns() const {
	return config.max_ins;
}

uint32_t GlobalConfig::getDelaysPerExec() const {
	return config.delays_per_exec;
}

uint32_t GlobalConfig::getMaxOverallMem() const {
	return config.max_overall_mem;
}

uint32_t GlobalConfig::getMemPerFrame() const {
	return config.mem_per_frame;
}

uint32_t GlobalConfig::getMinMemPerProcess() const {
	return config.min_mem_per_process;
}

uint32_t GlobalConfig::getMaxMemPerProcess() const {
	return config.max_mem_per_process;
}

uint32_t GlobalConfig::getMemPerProcess() const
{

	int memory = 0;

	if (config.min_mem_per_process == config.max_mem_per_process) {
		memory = config.min_mem_per_process;
	}
	else {
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dis(config.min_mem_per_process, config.min_mem_per_process);
		memory = dis(gen);
	}

	return memory;
}
