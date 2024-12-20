#include "CPUCore.h"

using namespace std;

int CPUCore::nextCPUCoreID = 0;

CPUCore::CPUCore() : totalCPUTicks(0), activeCPUTicks(0), quantumCounter(0)
{
	this->cpuCoreID = CPUCore::nextCPUCoreID;
	CPUCore::nextCPUCoreID += 1;
	
	thread tickCPUThread(&CPUCore::run, this);
	tickCPUThread.detach();
}

void CPUCore::assignProcess(shared_ptr<Process> process)
{
	this->process = process;
	this->quantumCounter = 0;

	if (this->process == nullptr) {
		this->availableFlag = true;
	}
	else {
		this->availableFlag = false;
	}

	//debugger:
	//cout << "Process assigned to CPU Core " << this->cpuCoreID << endl;
}

int CPUCore::getCPUCoreID() const {
	return this->cpuCoreID;
}

String CPUCore::getProcessName() const {
	return this->process->getName();
}

int CPUCore::getCommandCounter() const {
	return this->process->getCommandCounter();
}

int CPUCore::getLinesOfCode() const {
	return this->process->getLinesOfCode();
}

tm CPUCore::getArrivalTime() const {
	return this->process->getArrivalTime();
}

shared_ptr<Process> CPUCore::getProcess() const {
	return this->process;
}

void CPUCore::stop() {
	this->stopFlag = true;
}

bool CPUCore::isAvailable() const {
	return this->availableFlag;
}

void CPUCore::setAvailable(bool available) {
	this->availableFlag = available;
}

int CPUCore::getTotalCPUTicks() const {
    return this->totalCPUTicks;
}

int CPUCore::getActiveCPUTicks() const {
    return this->activeCPUTicks;
}

void CPUCore::run()
{
	this->stopFlag = false;

	while (this->stopFlag == false)
	{
		this->totalCPUTicks++;
		
		if (this->process != nullptr)
		{
			this->activeCPUTicks++;
			this->process->cpuCoreID = this->cpuCoreID;
			
			this->process->currentState = Process::ProcessState::RUNNING;
			//this->process->executeCurrentCommand();
			this->process->moveToNextLine();

			if (this->process->isFinished()) {
				// debugger:
				//std::cout << "Process " << this->process->getName() << " finished on core " << this->cpuCoreID << "\n";
				
				this->process->currentState = Process::ProcessState::FINISHED;

				this->assignProcess(nullptr);
				this->availableFlag = true;
			}
			else {
				String sched = GlobalConfig::getInstance()->getScheduler();

				if (sched == "rr") {
					this->process->currentState = Process::ProcessState::WAITING;
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
	this->availableFlag = true;

	//debugger:
	//std::cout << this->totalCPUTicks;
}