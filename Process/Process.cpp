#include "Process.h"
#include "Process.h"



Process::Process(int pid, String name, int totalLines, size_t reqMemory)
{
	this->requiredMemory = reqMemory;
	this->pid = pid;
	this->name = name;
	this->totalLines = totalLines;
	
	this->generateArrivalTime();

	this->commandCounter = 0;
	this->currentState = ProcessState::READY;
	//added
	this->storedAt = nullptr;
	this->inBackingstore = false;
}

void Process::manualAddCommand(String command)
{
	String toPrint = command;
	const std::shared_ptr<ICommand> print = std::make_shared<PrintCommand>(this->pid, toPrint);
	this->commandList.push_back(print);
}

void Process::addCommand(ICommand::CommandType commandType)
{
	if (commandType == ICommand::PRINT) {
		String toPrint = "Hello, World! From Process " + std::to_string(this->pid); // Debug output
		const std::shared_ptr<ICommand> print = std::make_shared<PrintCommand>(this->pid, toPrint);
		this->commandList.push_back(print);
	}
}

void Process::executeCurrentCommand() const
{
	this->commandList[this->commandCounter]->execute();
}

void Process::moveToNextLine() {
    std::lock_guard<std::mutex> lock(this->processMutex); // Add a mutex to Process class
    if (this->commandCounter < this->commandList.size()) {
        this->commandCounter++;
    }
}

bool Process::isFinished() const
{
	return this->commandCounter >= this->commandList.size();
}

int Process::getRemainingTime() const
{
	return this->commandCounter == this->commandList.size();
}

int Process::getCommandCounter() const
{
	return this->commandCounter;
}

//changes int to size_t
size_t Process::getLinesOfCode() const
{
	return this->commandList.size();
}

int Process::getPID() const
{
	return this->pid;
}

int Process::getCPUCoreID() const
{
	return this->cpuCoreID;
}

Process::ProcessState Process::getState() const
{
	return this->currentState;
}

String Process::getName() const
{
	return this->name;
}

std::tm Process::getArrivalTime() const
{
	return this->localArrivalTime;
}

String Process::getFormattedArrivalTime() const
{
	char buffer[64]; // Buffer for formatted time
	std::strftime(buffer, sizeof(buffer), "%m/%d/%Y, %I:%M:%S %p", &localArrivalTime);
	return String(buffer);
}

void Process::generateArrivalTime()
{
	auto now = std::chrono::system_clock::now();
	auto currentTime = std::chrono::system_clock::to_time_t(now);
	localtime_s(&localArrivalTime, &currentTime); // Store the arrival time
}

void Process::generateCommands()
{
	for (int i = 0; i < this->totalLines; i++) {
		this->addCommand(ICommand::CommandType::PRINT);
	}
}

void Process::setCPUCoreID(int cpuCoreID)
{
	this->cpuCoreID = cpuCoreID;
}


size_t Process::getRequiredMemory() const
{
	return this->requiredMemory;
}

void* Process::getStoredAt() const
{
	return this->storedAt;
}

void Process::setStoredAt(void* ptr)
{
	this->storedAt = ptr;
}

void Process::setInBackingStore(bool state)
{
	this->inBackingstore = state;
}

bool Process::isInBackingStore() const
{
	return this->inBackingstore;
}

void Process::setMemorySize(size_t size)
{
	this->requiredMemory = size;
}
