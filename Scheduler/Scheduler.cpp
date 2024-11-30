#include "Scheduler.h"
#include <mutex>



Scheduler* Scheduler::sharedInstance = nullptr;

Scheduler* Scheduler::getInstance()
{
	return sharedInstance;
}

void Scheduler::initialize()
{
	sharedInstance = new Scheduler();
}

void Scheduler::destroy()
{
	delete sharedInstance;
}

void Scheduler::stopScheduler()
{
	this->isRunning = false;
	if (schedulerThread.joinable()) {
		schedulerThread.join();
	}
}


std::string processStateToString(Process::ProcessState state) {
	switch (state) {
	case Process::ProcessState::RUNNING:
		return "RUNNING";
	case Process::ProcessState::WAITING:
		return "WAITING";
	case Process::ProcessState::FINISHED:
		return "FINISHED";
	default:
		return "UNKNOWN";
	}
}

void Scheduler::removeFinishedProcesses() {
	size_t j = readyQueue.size();
	std::lock_guard<std::mutex> lock(this->schedulerMutex);
	for (size_t i = 0; i < j; ) {
		std::shared_ptr<Process> process = processList[i];

		// Print the state for debugging purposes
		std::cout << "Process " << process->getName()
			<< " has state: " << processStateToString(process->getState()) << "\n";

		// Check if the process is finished
		if (process->getState() == Process::ProcessState::FINISHED) {
			// Deallocate memory if necessary
			this->memoryAllocator->deallocate(process);
			std::cout << "Memory deallocated for process " << process->getName() << ".\n";

			// Remove finished process from readyQueue
			readyQueue.erase(readyQueue.begin() + i);
		}
		else {
			// If not finished, move to the next process
			++i;
		}
	}
}

void Scheduler::reallocateMemoryInWaitList() { //attempt to reallocate
	std::lock_guard<std::mutex> lock(this->schedulerMutex);

	//for (size_t i = 0; i < waitListQueue.size(); ) {
	//	std::shared_ptr<Process> process = waitListQueue[i];
	//	void* allocatedMemory = this->memoryAllocator->allocate(process);

	//	if (allocatedMemory != nullptr) {
	//		process->setStoredAt(allocatedMemory);
	//		this->readyQueue.push_back(process);  // Move to readyQueue
	//		waitListQueue.erase(waitListQueue.begin() + i);  // Erase from waitListQueue
	//		std::cout << "Process " << process->getName()
	//			<< " REallocated memory and moved to ready queue.\n";  // Debugging log
	//	}
	//	else {
	//		++i;  // move to the next process if memory allocation failed
	//	}
	//}
	if (!waitListQueue.empty()) {
		std::shared_ptr<Process> process = waitListQueue.front();  // Get the first process
		void* allocatedMemory = this->memoryAllocator->allocate(process);

		if (allocatedMemory != nullptr) {
			// Successfully allocated memory
			process->setStoredAt(allocatedMemory);  // Allocate memory
			this->readyQueue.push_back(process);    // Move to ready queue
			waitListQueue.erase(waitListQueue.begin()); // Remove from waitListQueue
			std::cout << "Process " << process->getName()
				<< " REallocated memory and moved to ready queue.\n";  // Debugging log
		}
		else {
			std::cout << "Memory allocation failed for process " << process->getName() << ".\n";
			// The process will remain in the waitlist to try again later
		}
	}
}



void Scheduler::startSchedulerThread(String scheduler, int delay, int quantum)
{
	if (this->isRunning == false) {
		this->isRunning = true;

		if (scheduler == "fcfs") {
			schedulerThread = std::thread(&Scheduler::runFCFSScheduler, this, delay);

			
		}
		else if (scheduler == "rr") {
			schedulerThread = std::thread(&Scheduler::runRoundRobinScheduler, this, delay, quantum);

		}

		schedulerThread.detach();
	}
}

std::shared_ptr<Process> Scheduler::createUniqueProcess()
{
	int totalLines = GlobalConfig::getInstance()->getRandomInstructionCount();
	int pidCounter = ConsoleManager::getInstance()->getProcessTableSize();
	pidCounter += 1;

	String name = this->generateUniqueProcessName(pidCounter);

	std::shared_ptr<Process> existingProcess = this->findProcess(name);

	if (existingProcess != nullptr) {
		return existingProcess;
	}
	else {
		//changes added requiredMemory
		size_t requiredMemory = GlobalConfig::getInstance()->getMemPerProcess();
		std::shared_ptr<Process> newProcess = std::make_shared<Process>(pidCounter, name, totalLines, requiredMemory);
		////added memory allocation
		//void* allocatedMemory = this->memoryAllocator->allocate(newProcess);
		//if (allocatedMemory == nullptr) { //if not allocated
		//	return nullptr;
		//}
		//newProcess->setStoredAt(allocatedMemory);

		newProcess->generateCommands();

		this->addProcessToReadyQueue(newProcess);

		// debugger:
		//std::cout << "Process " << newProcess->getName() << " added to ready queue" << std::endl;

		return newProcess;
	}
}

std::shared_ptr<Process> Scheduler::findProcess(String name) const
{
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

	std::cout << "CPU Utilization: " << cpuUtil << "%" << std::endl;
	std::cout << "Cores used: " << this->numCPU - availableCPUCount << std::endl;
	std::cout << "Cores available: " << availableCPUCount << std::endl;
	std::cout << std::endl;

	for (int i = 0; i < 62; i++) {
		std::cout << "-";
	}
	std::cout << std::endl;

	std::cout << "Running processes:" << std::endl;

	for (int i = 0; i < this->processList.size(); i++) {
		std::shared_ptr<Process> process = this->processList[i];
		if (process->getState() == Process::ProcessState::RUNNING || process->getCPUCoreID() == -1 || process->getState() == Process::ProcessState::WAITING) {
			String processName = process->getName();
			int commandCounter = process->getCommandCounter();
			int linesOfCode = process->getLinesOfCode();
			int cpuCoreID = process->getCPUCoreID();

			auto arrivalTime = process->getArrivalTime();

			// format the arrival time : (mm/dd/yyyy, hh:mm:ss AM/PM)
			char buffer[64];
			std::strftime(buffer, sizeof(buffer), "%m/%d/%Y, %I:%M:%S %p", &arrivalTime);

			if (cpuCoreID == -1) {
				std::cout << processName << "\t" << "(" << buffer << ")" << "\t" << "Core: N/A" << "\t" << commandCounter << " / " << linesOfCode << std::endl;
			}
			else {
				std::cout << processName << "\t" << "(" << buffer << ")" << "\t" << "Core: " << cpuCoreID << " " << "\t" << commandCounter << " / " << linesOfCode << std::endl;
			}
		}
	}
	
	// for (int i = 0; i < this->cpuCoreList.size(); i++) {
	// 	std::shared_ptr<CPUCore> cpuCore = this->cpuCoreList[i];
	// 	if (!cpuCore->isAvailable()) {
	// 		String processName = cpuCore->getProcessName();
	// 		int commandCounter = cpuCore->getCommandCounter();
	// 		int linesOfCode = cpuCore->getLinesOfCode();
	// 		int cpuCoreID = cpuCore->getCPUCoreID();

	// 		auto arrivalTime = cpuCore->getArrivalTime();

	// 		// format the arrival time : (mm/dd/yyyy, hh:mm:ss AM/PM)
	// 		char buffer[64];
	// 		std::strftime(buffer, sizeof(buffer), "%m/%d/%Y, %I:%M:%S %p", &arrivalTime);



	// 		std::cout << processName << "\t" << "(" << buffer << ")" << "\t" << "Core: " << cpuCoreID << " " << "\t" << commandCounter << " / " << linesOfCode << std::endl;
	// 	}
	// 	/*else {
	// 		std::cout << "Core " << cpuCore->getCPUCoreID() << ": " << "Idle" << std::endl;
	// 	}*/
	// }

	std::cout << std::endl;
	for (int i = 0; i < 62; i++) {
		std::cout << "-";
	}
	std::cout << std::endl;

	std::cout << "Finished processes:" << std::endl;

	for (int i = 0; i < this->processList.size(); i++) {
		std::shared_ptr<Process> process = this->processList[i];
		if (process->getState() == Process::ProcessState::FINISHED) {
			
			String processName = process->getName();
			int commandCounter = process->getCommandCounter();
			int linesOfCode = process->getLinesOfCode();
			int cpuCoreID = process->getCPUCoreID();

			auto arrivalTime = process->getArrivalTime();

			// format the arrival time : (mm/dd/yyyy, hh:mm:ss AM/PM)
			char buffer[64];
			std::strftime(buffer, sizeof(buffer), "%m/%d/%Y, %I:%M:%S %p", &arrivalTime);

			std::cout << processName << "\t" << "(" << buffer << ")" << "\t" << "FINISHED" << "\t" << commandCounter << " / " << linesOfCode << std::endl;
		}
	}
}

String Scheduler::generateUniqueProcessName(int id)
{
	std::stringstream ss;
	ss << "Process_" << id;
	return ss.str();
}

void Scheduler::addProcessToReadyQueue(std::shared_ptr<Process> process)
{
	//std::lock_guard<std::mutex> lock(this->schedulerMutex);
	//changes added
	//void* allocatedMemory = this->memoryAllocator->allocate(process);
	//if (allocatedMemory != nullptr) {//if allocated
	//	process->setStoredAt(allocatedMemory);
	//	this->readyQueue.push_back(process);
	//	//std::cout << "Process " << process->getName() << " added to ready queue.\n"; //check
	//}
	//else {
	//	//std::cerr << "Memory allocation failed for process " << process->getName()
	//	//	<< ". Adding to waiting queue.\n"; //check
	//	this->waitListQueue.push_back(process);
	//}
	//this->processList.push_back(process);

	//old
	this->readyQueue.push_back(process);
	this->processList.push_back(process);

	// debugger:
	//std::cout << "Process " << process->getName() << " added to ready queue" << std::endl;
}

void Scheduler::runFCFSScheduler(int delay)
{
	while (this->isRunning) {
		//std::lock_guard<std::mutex> lock(schedulerMutex);

		//added
		/*this->removeFinishedProcesses();
		this->reallocateMemoryInWaitList();*/

		for (int i = 0; i < this->cpuCoreList.size(); i++) {
			std::shared_ptr<CPUCore> cpuCore = this->cpuCoreList[i];

			if (cpuCore->isAvailable()) {
				if (!this->readyQueue.empty()) {
					std::shared_ptr<Process> process = this->readyQueue.front();
					void* allocatedMemory = this->memoryAllocator->allocate(process);
					if (allocatedMemory != nullptr) {//if allocated
						process->setStoredAt(allocatedMemory);
						this->memoryAllocator->deallocate(process);
						/*this->readyQueue.push_back(process);*/
						//std::cout << "Process " << process->getName() << " added to ready queue.\n"; //check
					}
					/*else {
						this->backingStore->saveProcess(process);
						this
					}*/

					this->readyQueue.erase(this->readyQueue.begin());

					cpuCore->assignProcess(process);

					/*while (!process->isFinished()) {
						process->executeCurrentCommand();
						process->moveToNextLine();

						Sleep(delay);
					}*/
				}
			}



			/*if (this->readyQueue.size() > 0) {
				std::shared_ptr<Process> process = this->readyQueue.front();
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
			std::shared_ptr<CPUCore> cpuCore = this->cpuCoreList[i];

			if (cpuCore->isAvailable()) {
				if (!this->readyQueue.empty()) {
					std::shared_ptr<Process> process = this->readyQueue.front();
					this->readyQueue.erase(this->readyQueue.begin());

					if (process->getStoredAt() == nullptr && process->isInBackingStore()) {
						backingStore->loadProcess(process);
					}

					void* allocatedMemory = this->memoryAllocator->allocate(process);
					if (allocatedMemory != nullptr) {
						process->setStoredAt(allocatedMemory);
						cpuCore->assignProcess(process);

						/*quantumCounter++;*/
						if (cpuCore->getProcess()->getState() == Process::ProcessState::WAITING) {
							cpuCore->getProcess()->setCPUCoreID(-1);
							this->readyQueue.push_back(process);
						}

						if (cpuCore->getProcess()->getState() == Process::ProcessState::FINISHED) {
							cpuCore->getProcess()->setCPUCoreID(-1);
							this->memoryAllocator->deallocate(process);
							/*this->readyQueue.erase(this->readyQueue.begin());*/
						}

					}
					else {
						swapOutProcess();
						allocatedMemory = this->memoryAllocator->allocate(process);
						if (allocatedMemory != nullptr) {
							process->setStoredAt(allocatedMemory);
							cpuCore->assignProcess(process);
						}
						else {
							this->readyQueue.push_back(process);
						}
					}


				}
			}
			
		}
	}
}



Scheduler::Scheduler()
{
	this->numCPU = GlobalConfig::getInstance()->getNumCPU();
	this->bacthProcessFrequency = GlobalConfig::getInstance()->getBatchProcessFreq();
	this->minInstructions = GlobalConfig::getInstance()->getMinIns();
	this->maxInstructions = GlobalConfig::getInstance()->getMaxIns();
	//added
	this->backingStore = std::make_unique<BackingStore>("../BackingStore/");
	this->overAllMemory = GlobalConfig::getInstance()->getMaxOverallMem();
	this->memPerFrame = GlobalConfig::getInstance()->getMemPerFrame();
	this->memPerProc = GlobalConfig::getInstance()->getMemPerProcess();
	this->baseAddress = new char[overAllMemory];
	this->memoryAllocator = std::make_unique<FlatMemoryAllocator>(baseAddress, overAllMemory);

	for (int i = 0; i < this->numCPU; i++) {
		std::shared_ptr<CPUCore> cpuCore = std::make_shared<CPUCore>();
		this->cpuCoreList.push_back(cpuCore);
	}

	String scheduler = GlobalConfig::getInstance()->getScheduler();
	int quantum = GlobalConfig::getInstance()->getQuantumCycles();
	this->startSchedulerThread(scheduler, this->bacthProcessFrequency, quantum);
}


void Scheduler::swapOutProcess() {
	if (this->readyQueue.empty()) return;

	// Select a process to swap out (e.g., based on LRU or LFU)
	std::shared_ptr<Process> processToSwap = this->readyQueue.front();
	this->readyQueue.pop_back();

	// Free its memory
	if (processToSwap->getStoredAt() != nullptr) {
		this->memoryAllocator->deallocate(processToSwap);
		processToSwap->setStoredAt(nullptr);
	}

	// Move the process to the backing store
	this->backingStore->saveProcess(processToSwap);
	std::cout << "Swapped out process " << processToSwap->getName()
		<< " to the backing store.\n";
}