#include "CPUCore.h"

int CPUCore::nextCPUCoreID = 0;

CPUCore::CPUCore()
{
	this->cpuCoreID = CPUCore::nextCPUCoreID;
	CPUCore::nextCPUCoreID += 1;

	std::thread tickCPUThread(&CPUCore::run, this);
	tickCPUThread.detach();
}

void CPUCore::assignProcess(std::shared_ptr<Process> process)
{
	this->process = process;

	if (this->process == nullptr) {
		this->availableFlag = true;
	}
	else {
		this->availableFlag = false;
	}

	//debugger:
	//std::cout << "Process assigned to CPU Core " << this->cpuCoreID << std::endl;

}

int CPUCore::getCPUCoreID() const
{
	return this->cpuCoreID;
}

String CPUCore::getProcessName() const
{
	return this->process->getName();
}

int CPUCore::getCommandCounter() const
{
	return this->process->getCommandCounter();
}

int CPUCore::getLinesOfCode() const
{
	return this->process->getLinesOfCode();
}

std::tm CPUCore::getArrivalTime() const
{
	return this->process->getArrivalTime();
}

std::shared_ptr<Process> CPUCore::getProcess() const
{
	return this->process;
}

void CPUCore::stop()
{
	this->stopFlag = true;
}

bool CPUCore::isAvailable() const
{
	return this->availableFlag;
}

void CPUCore::setAvailable(bool available)
{
	this->availableFlag = available;
}




void CPUCore::run()
{
	this->stopFlag = false;

	while (this->stopFlag == false) {
		if (this->process != nullptr) {
			this->process->cpuCoreID = this->cpuCoreID;
			//this->process->executeCurrentCommand();
			this->process->moveToNextLine();

			if (this->process->isFinished()) {
				// debugger:
				//std::cout << "Process " << this->process->getName() << " finished on core " << this->cpuCoreID << "\n";
				
				this->process->currentState = Process::ProcessState::FINISHED;

				this->assignProcess(nullptr);
				this->availableFlag = true;

				
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}

	this->availableFlag = true;
}
