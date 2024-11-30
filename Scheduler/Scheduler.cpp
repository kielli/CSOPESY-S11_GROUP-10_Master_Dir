#include "Scheduler.h"
#include "../Memory/FlatMemoryAllocator.h"
//#include "../Memory/PagingAllocator.h"

using namespace std;

Scheduler* Scheduler::sharedInstance = nullptr;

Scheduler* Scheduler::getInstance() {
	return sharedInstance;
}

void Scheduler::initialize() {
	if (sharedInstance == nullptr) {
		sharedInstance = new Scheduler();
	}
}

void Scheduler::destroy() {
	if (sharedInstance) {
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}


void Scheduler::stopScheduler()
{
	this->isRunning = false;
	if (schedulerThread.joinable()) {
		schedulerThread.join();
	}
}

void Scheduler::startSchedulerThread(String scheduler, int delay, int quantum)
{
	if (this->isRunning == false) {
		this->isRunning = true;

		if (scheduler == "fcfs") {
			schedulerThread = thread(&Scheduler::runFCFSScheduler, this, delay);
		}
		else if (scheduler == "rr") {
			schedulerThread = thread(&Scheduler::runRoundRobinScheduler, this, delay, quantum);
		}

		schedulerThread.detach();
	}
}

shared_ptr<Process> Scheduler::createUniqueProcess()
{
	int totalLines = GlobalConfig::getInstance()->getRandomInstructionCount();
	int pidCounter = ConsoleManager::getInstance()->getProcessTableSize();
	pidCounter += 1;

	String name = this->generateUniqueProcessName(pidCounter);

	shared_ptr<Process> existingProcess = this->findProcess(name);

	if (existingProcess != nullptr) {
		return existingProcess;
	}
	else {
		shared_ptr<Process> newProcess = make_shared<Process>(pidCounter, name, totalLines, GlobalConfig::getInstance()->getMemPerProcess());
		newProcess->generateCommands();

		this->addProcessToReadyQueue(newProcess);

		// debugger:
		//cout << "Process " << newProcess->getName() << " added to ready queue" << endl;

		return newProcess;
	}
}

shared_ptr<Process> Scheduler::findProcess(String name) const {
	return ConsoleManager::getInstance()->findProcess(name);

}

void Scheduler::displaySchedulerStatus()
{
	int availableCPUCount = 0;

    for (int i = 0; i < this->cpuCoreList.size(); i++) {
        std::shared_ptr<CPUCore> cpuCore = this->cpuCoreList[i];
        if (cpuCore->isAvailable()) {
            availableCPUCount += 1;
        }
    }

    float util = (((float)(this->numCPU - availableCPUCount) / (float)this->numCPU) * 100);

    std::cout << "CPU Utilization: " << util << "%" << std::endl;
    std::cout << "Cores used: " << this->numCPU - availableCPUCount << std::endl;
    std::cout << "Cores available: " << availableCPUCount << std::endl;
    std::cout << std::endl;

	for (int i = 0; i < 62; i++) {
		cout << "-";
	}
	cout << endl;

	cout << "Running processes:" << endl;

	for (int i = 0; i < this->processList.size(); i++) {
		shared_ptr<Process> process = this->processList[i];
		if (process->getState() == Process::ProcessState::RUNNING || process->getCPUCoreID() == -1 || process->getState() == Process::ProcessState::WAITING) {
			String processName = process->getName();
			int commandCounter = process->getCommandCounter();
			int linesOfCode = process->getLinesOfCode();
			int cpuCoreID = process->getCPUCoreID();

			auto arrivalTime = process->getArrivalTime();

			// format the arrival time : (mm/dd/yyyy, hh:mm:ss AM/PM)
			char buffer[64];
			strftime(buffer, sizeof(buffer), "%m/%d/%Y, %I:%M:%S %p", &arrivalTime);

			if (cpuCoreID == -1) {
				cout << processName << "\t" << "(" << buffer << ")" << "\t" << "Core: N/A" << "\t" << commandCounter << " / " << linesOfCode << endl;
			}
			else {
				cout << processName << "\t" << "(" << buffer << ")" << "\t" << "Core: " << cpuCoreID << " " << "\t" << commandCounter << " / " << linesOfCode << endl;
			}
		}
	}


	cout << endl;
	for (int i = 0; i < 62; i++) {
		cout << "-";
	}
	cout << endl;

	cout << "Finished processes:" << endl;

	for (int i = 0; i < this->processList.size(); i++) {
		shared_ptr<Process> process = this->processList[i];
		if (process->getState() == Process::ProcessState::FINISHED) {
			
			String processName = process->getName();
			int commandCounter = process->getCommandCounter();
			int linesOfCode = process->getLinesOfCode();
			int cpuCoreID = process->getCPUCoreID();

			auto arrivalTime = process->getArrivalTime();

			// format the arrival time : (mm/dd/yyyy, hh:mm:ss AM/PM)
			char buffer[64];
			strftime(buffer, sizeof(buffer), "%m/%d/%Y, %I:%M:%S %p", &arrivalTime);

			cout << processName << "\t" << "(" << buffer << ")" << "\t" << "FINISHED" << "\t" << linesOfCode << " / " << linesOfCode << endl;
		}
	}
}

void Scheduler::processSMI()
{
	cout << "\n---------------------------------------------------------------" << endl;
	cout << "| PROCESS-SMI v01.00                 Driver Version: 01.00    |" << endl;
	cout << "---------------------------------------------------------------" << endl;

	int useCPU = 100 / this->numCPU;
	int usageCPU = 0;

	for (int i = 0; i < this->cpuCoreList.size(); i++) {
		shared_ptr<CPUCore> cpuCore = this->cpuCoreList[i];
		if (cpuCore->getProcess() != nullptr) {
			usageCPU += useCPU;
		}
	}

	cout << "CPU-Util: " << usageCPU << "%" << endl;

	this->memoryAllocator->printProcesses();
}

void Scheduler::vmStat()
{
	this->memoryAllocator->vmstat();
}

std::shared_ptr<IMemoryAllocator> Scheduler::getMemoryAllocator() const
{
    return this->memoryAllocator;
}

String Scheduler::generateUniqueProcessName(int id)
{
	stringstream ss;
	ss << "Process_" << id;
	return ss.str();
}

void Scheduler::addProcessToReadyQueue(shared_ptr<Process> process)
{
	this->readyQueue.push_back(process);
	this->processList.push_back(process);

	// debugger:
	//cout << "Process " << process->getName() << " added to ready queue" << endl;
}

int Scheduler::getTotalCPUTicks()
{
    int totalTicks = 0;

    for (int i = 0; i < this->cpuCoreList.size(); i++) {
        std::shared_ptr<CPUCore> cpuCore = this->cpuCoreList[i];
        totalTicks += cpuCore->getTotalTicks();
    }

    return totalTicks;
}

int Scheduler::getInactiveCPUTicks()
{
	int inactiveTicks = 0;

	for (int i = 0; i < this->cpuCoreList.size(); i++) {
		std::shared_ptr<CPUCore> cpuCore = this->cpuCoreList[i];
		inactiveTicks += cpuCore->getInactiveTicks();
	}

	return inactiveTicks;
}

// void Scheduler::runFCFSScheduler(int delay)
// {
// 	while (this->isRunning)
// 	{

// 		for (int i = 0; i < this->cpuCoreList.size(); i++) {
// 			shared_ptr<CPUCore> cpuCore = this->cpuCoreList[i];

// 			if (cpuCore->isAvailable()) {
// 				if (!this->readyQueue.empty()) {
// 					shared_ptr<Process> process = this->readyQueue.front();
// 					this->readyQueue.erase(this->readyQueue.begin());

// 					cpuCore->assignProcess(process);
// 				}
// 			}
// 		}
// 	}
// }


// void Scheduler::runRoundRobinScheduler(int delay, int quantum)
// {
// 	int quantumCounter = 0;

//     while(this->isRunning) {
// 		for (int i = 0; i < this->cpuCoreList.size(); i++) {
// 			std::shared_ptr<CPUCore> cpuCore = this->cpuCoreList[i];

// 			if (cpuCore->isAvailable()) {
// 				if (!this->readyQueue.empty()) {
// 					std::shared_ptr<Process> process = this->readyQueue.front();
// 					this->readyQueue.erase(this->readyQueue.begin());

// 					cpuCore->assignProcess(process);

// 					if(cpuCore->getProcess()->getState() == Process::ProcessState::WAITING) {
// 						cpuCore->getProcess()->setCPUCoreID(-1);
// 						this->readyQueue.push_back(process);
// 					}
// 				}
// 			}
			
// 		}
// 	}
// }

void Scheduler::runFCFSScheduler(int delay)
{
	while (this->isRunning)
	{

		for (int i = 0; i < this->cpuCoreList.size(); i++) {
			shared_ptr<CPUCore> cpuCore = this->cpuCoreList[i];

			if (cpuCore->isAvailable()) {
				if (cpuCore->getProcess() != nullptr && cpuCore->getProcess()->getState() == Process::ProcessState::FINISHED) {
					this->memoryAllocator->deallocate(cpuCore->getProcess());
					cpuCore->assignProcess(nullptr);
				}

				if (!this->readyQueue.empty()) {
					shared_ptr<Process> process = this->readyQueue.front();
					this->readyQueue.erase(this->readyQueue.begin());

					cpuCore->assignProcess(process);
				}
			}
		}
	}
}


void Scheduler::runRoundRobinScheduler(int delay, int quantum)
{
	auto startTime = std::chrono::steady_clock::now();
	this->cycleCounter = 0;

    while(this->isRunning) {
		auto currentTime = std::chrono::steady_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

		// Check if quantum limit exceeded
		if (elapsedTime >= quantum) {
			for (int i = 0; i < this->cpuCoreList.size(); i++) {
				std::shared_ptr<CPUCore> cpuCore = this->cpuCoreList[i];

				// Check if process is finished
				if (cpuCore->getProcess() != nullptr) {
					this->readyQueue.push_back(cpuCore->getProcess());
					cpuCore->assignProcess(nullptr);
					cpuCore->setAvailable(true);
					this->isRunning = true;	// Continue the loop
				}
			}
			startTime = std::chrono::steady_clock::now();	// Reset the start time
			this->cycleCounter += 1;
		}


		// Assign Process to CPU Core
		for (int i = 0; i < this->cpuCoreList.size(); i++) {
			shared_ptr<CPUCore> cpuCore = this->cpuCoreList[i];

			std::lock_guard<std::mutex> lock(this->schedulerMutex);

			// process
			std::shared_ptr<Process> process = cpuCore->getProcess();

			if (process != nullptr && process->getState() == Process::ProcessState::FINISHED) {
				this->memoryAllocator->deallocate(cpuCore->getProcess());
				cpuCore->assignProcess(nullptr);
				cpuCore->setAvailable(true);
			}

			if (cpuCore->isAvailable() && !this->readyQueue.empty()) {
				std::shared_ptr<Process> process = this->readyQueue.front();

				if (this->memoryAllocator->allocate(process)) {
					/*cpuCore->getProcess()->setCPUCoreID(cpuCore->getCPUCoreID());*/
					cpuCore->assignProcess(process);
					this->readyQueue.erase(this->readyQueue.begin());
					this->isRunning = true;
				}
				else {
					this->readyQueue.erase(this->readyQueue.begin());
					this->readyQueue.push_back(process);
				}

				startTime = std::chrono::steady_clock::now();
			}
		}
	}
}


// Constructor
Scheduler::Scheduler()
{
	this->numCPU = GlobalConfig::getInstance()->getNumCPU();
	this->bacthProcessFrequency = GlobalConfig::getInstance()->getBatchProcessFreq();
	this->minInstructions = GlobalConfig::getInstance()->getMinIns();
	this->maxInstructions = GlobalConfig::getInstance()->getMaxIns();

	for (int i = 0; i < this->numCPU; i++) {
		shared_ptr<CPUCore> cpuCore = make_shared<CPUCore>();
		this->cpuCoreList.push_back(cpuCore);
	}

	String scheduler = GlobalConfig::getInstance()->getScheduler();
	int quantum = GlobalConfig::getInstance()->getQuantumCycles();
	this->startSchedulerThread(scheduler, this->bacthProcessFrequency, quantum);

	// Memory Manager
	int maxOverAllMemory = GlobalConfig::getInstance()->getMaxOverallMem();
	int memPerFrame = GlobalConfig::getInstance()->getMemPerFrame();

	if (maxOverAllMemory == memPerFrame) {
		this->memoryAllocator = std::make_shared<FlatMemoryAllocator>(maxOverAllMemory);
	}
	// else {
	// 	this->memoryAllocator = std::make_shared<PagingAllocator>(maxOverAllMemory, memPerFrame);
	// }
}


