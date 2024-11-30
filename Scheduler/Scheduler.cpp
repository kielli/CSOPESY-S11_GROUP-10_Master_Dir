#include "Scheduler.h"
#include "../Memory/FlatMemoryAllocator.h"
#include "../Memory/PagingAllocator.h"
#include <mutex>

using namespace std;

Scheduler* Scheduler::sharedInstance = nullptr;

Scheduler* Scheduler::getInstance() {
	return sharedInstance;
}

void Scheduler::initialize() {
	sharedInstance = new Scheduler();
}

void Scheduler::destroy() {
	delete sharedInstance;
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
		if (this->cpuCoreList[i]->isAvailable()) {
			availableCPUCount += 1;
		}
	}

	float cpuUtil = (this->numCPU - availableCPUCount) / this->numCPU * 100;

	cout << "CPU Utilization: " << cpuUtil << "%" << endl;
	cout << "Cores used: " << this->numCPU - availableCPUCount << endl;
	cout << "Cores available: " << availableCPUCount << endl;
	cout << endl;

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
	
	// for (int i = 0; i < this->cpuCoreList.size(); i++) {
	// 	shared_ptr<CPUCore> cpuCore = this->cpuCoreList[i];
	// 	if (!cpuCore->isAvailable()) {
	// 		String processName = cpuCore->getProcessName();
	// 		int commandCounter = cpuCore->getCommandCounter();
	// 		int linesOfCode = cpuCore->getLinesOfCode();
	// 		int cpuCoreID = cpuCore->getCPUCoreID();

	// 		auto arrivalTime = cpuCore->getArrivalTime();

	// 		// format the arrival time : (mm/dd/yyyy, hh:mm:ss AM/PM)
	// 		char buffer[64];
	// 		strftime(buffer, sizeof(buffer), "%m/%d/%Y, %I:%M:%S %p", &arrivalTime);



	// 		cout << processName << "\t" << "(" << buffer << ")" << "\t" << "Core: " << cpuCoreID << " " << "\t" << commandCounter << " / " << linesOfCode << endl;
	// 	}
	// 	/*else {
	// 		cout << "Core " << cpuCore->getCPUCoreID() << ": " << "Idle" << endl;
	// 	}*/
	// }

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

			cout << processName << "\t" << "(" << buffer << ")" << "\t" << "FINISHED" << "\t" << commandCounter << " / " << linesOfCode << endl;
		}
	}
}

String Scheduler::generateUniqueProcessName(int id)
{
	stringstream ss;
	ss << "Process_" << id;
	return ss.str();
}

void Scheduler::addProcessToReadyQueue(shared_ptr<Process> process)
{
	//lock_guard<mutex> lock(this->schedulerMutex);
	this->readyQueue.push_back(process);
	this->processList.push_back(process);

	// debugger:
	//cout << "Process " << process->getName() << " added to ready queue" << endl;
}

void Scheduler::runFCFSScheduler(int delay)
{
	while (this->isRunning)
	{
		//lock_guard<mutex> lock(schedulerMutex);

		for (int i = 0; i < this->cpuCoreList.size(); i++) {
			shared_ptr<CPUCore> cpuCore = this->cpuCoreList[i];

			if (cpuCore->isAvailable()) {
				if (!this->readyQueue.empty()) {
					shared_ptr<Process> process = this->readyQueue.front();
					this->readyQueue.erase(this->readyQueue.begin());

					cpuCore->assignProcess(process);
					
					do{
						if(process->isFinished()){

						}
					}while(!process->isFinished());
				}
			}

			/*if (this->readyQueue.size() > 0) {
				shared_ptr<Process> process = this->readyQueue.front();
				this->readyQueue.pop();

				int cpuCoreID = process->getCPUCoreID();
				this->cpuCoreList[cpuCoreID]->assignProcess(process);

				ConsoleManager::getInstance()->addProcess(process);
				ConsoleManager::getInstance()->switchConsole(SCHEDULING_CONSOLE_NAME);

				while (!process->isFinished()) {
					process->executeCurrentCommand();
					process->moveToNextLine();

					ConsoleManager::getInstance()->drawConsole();
					Sleep(delay);
				}

				this->cpuCoreList[cpuCoreID]->removeProcess();
			}*/
		}
	}
}


void Scheduler::runRoundRobinScheduler(int delay, int quantum)
{
	int quantumCounter = 0;

    while(this->isRunning) {
		for (int i = 0; i < this->cpuCoreList.size(); i++) {
			shared_ptr<CPUCore> cpuCore = this->cpuCoreList[i];

			if (cpuCore->isAvailable()) {
				if (!this->readyQueue.empty()) {
					shared_ptr<Process> process = this->readyQueue.front();
					this->readyQueue.erase(this->readyQueue.begin());

					cpuCore->assignProcess(process);

					if(cpuCore->getProcess()->getState() == Process::ProcessState::WAITING) {
						cpuCore->getProcess()->setCPUCoreID(-1);
						this->readyQueue.push_back(process);
					}
				}
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
}