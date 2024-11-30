#include "GlobalConfig.h"


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
	config.mem_per_process = 0;
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

void GlobalConfig::loadConfigFile(String& filename) {

	try {
		std::ifstream file(filename);
		if (!file.is_open()) {
			throw std::runtime_error("Error: Could not open file " + filename);
		}
		else {
			String line;
			while (std::getline(file, line)) {
				if (!parseConfigFile(line)) {
					throw std::runtime_error("Error: Could not parse line " + line);
					file.close();
					break;
				}
			}

			file.close();
		}
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
	}
}



bool GlobalConfig::parseConfigFile(String& line) {
	bool isParsed = false;

	std::istringstream iss(line);
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
				throw std::invalid_argument("num-cpu must be between 1 and 128");
			}
		}
		else if (key == "scheduler") {
			String value;
			iss >> std::quoted(value); // Handles quoted strings
			if (value == "rr" || value == "fcfs") {
				config.scheduler = value;
				isParsed = true;
			}
			else {
				throw std::invalid_argument("scheduler must be \"rr\" or \"fcfs\"");
			}
		}
		else if (key == "quantum-cycles") {
			uint32_t value;
			if (iss >> value && value >= 1) {
				config.quantum_cycles = value;
				isParsed = true;
			}
			else {
				throw std::invalid_argument("quantum-cycles must be >= 1");
			}
		}
		else if (key == "batch-process-freq") {
			uint32_t value;
			if (iss >> value && value >= 1) {
				config.batch_process_freq = value;
				isParsed = true;
			}
			else {
				throw std::invalid_argument("batch-process-freq must be >= 1");
			}
		}
		else if (key == "min-ins") {
			uint32_t value;
			if (iss >> value && value >= 1) {
				config.min_ins = value;
				isParsed = true;
			}
			else {
				throw std::invalid_argument("min-ins must be >= 1");
			}
		}
		else if (key == "max-ins") {
			uint32_t value;
			if (iss >> value && value >= config.min_ins) {
				config.max_ins = value;
				isParsed = true;
			}
			else {
				throw std::invalid_argument("max-ins must be >= min-ins");
			}
		}
		else if (key == "delay-per-exec") {
			uint32_t value;
			if (iss >> value) {
				config.delays_per_exec = value;
				isParsed = true;
			}
			else {
				throw std::invalid_argument("Invalid delay-per-exec value");
			}
		}
		else if (key == "max-overall-mem") {
			uint32_t value;
			if (iss >> value) {
				config.max_overall_mem = value;
				isParsed = true;
			}
			else {
				throw std::invalid_argument("Invalid max-overall-mem value");
			}
		}
		else if (key == "mem-per-frame") {
			uint32_t value;
			if (iss >> value) {
				config.mem_per_frame = value;
				isParsed = true;
			}
			else {
				throw std::invalid_argument("Invalid mem-per-frame value");
			}
		}
		else if (key == "mem-per-proc") {
			uint32_t value;
			if (iss >> value) {
				config.mem_per_process = value;
				isParsed = true;
			}
			else {
				throw std::invalid_argument("Invalid mem-per-process value");
			}
		}
		else {
			throw std::invalid_argument("Unknown configuration key: " + key);
		}
	}
	catch (const std::invalid_argument& e) {
		std::cerr << "Error: " << e.what() << " in line: " << line << std::endl;
		isParsed = false;
	}


	return isParsed;
}

void GlobalConfig::printConfig() const {
	std::cout << "Number of CPUs: " << static_cast<int>(config.num_cpu) << std::endl;
	std::cout << "Scheduler: " << config.scheduler << std::endl;
	std::cout << "Quantum Cycles: " << config.quantum_cycles << std::endl;
	std::cout << "Batch Process Frequency: " << config.batch_process_freq << std::endl;
	std::cout << "Min Instructions: " << config.min_ins << std::endl;
	std::cout << "Max Instructions: " << config.max_ins << std::endl;
	std::cout << "Delays per Execution: " << config.delays_per_exec << std::endl;
	std::cout << "Max Overall Memory: " << config.max_overall_mem << std::endl;
	std::cout << "Memory per Frame: " << config.mem_per_frame << std::endl;
	std::cout << "Memory per Process: " << config.mem_per_process << std::endl;
}

int GlobalConfig::getRandomInstructionCount() const
{
	int linesOfCode = 0;

	if (config.min_ins == config.max_ins) {
		linesOfCode = config.min_ins;
	}
	else {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(config.min_ins, config.max_ins);
		linesOfCode = dis(gen);
	}

	return linesOfCode;
}

uint16_t GlobalConfig::getNumCPU() const
{
	return config.num_cpu;
}

String GlobalConfig::getScheduler() const
{
	return config.scheduler;
}

uint32_t GlobalConfig::getQuantumCycles() const
{
	return config.quantum_cycles;
}

uint32_t GlobalConfig::getBatchProcessFreq() const
{
	return config.batch_process_freq;
}

uint32_t GlobalConfig::getMinIns() const
{
	return config.min_ins;
}

uint32_t GlobalConfig::getMaxIns() const
{
	return config.max_ins;
}

uint32_t GlobalConfig::getDelaysPerExec() const
{
	return config.delays_per_exec;
}

uint32_t GlobalConfig::getMaxOverallMem() const
{
	return config.max_overall_mem;
}

uint32_t GlobalConfig::getMemPerFrame() const
{
	return config.mem_per_frame;
}

uint32_t GlobalConfig::getMemPerProcess() const
{
	return config.mem_per_process;
}


