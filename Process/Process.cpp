#include "Process.h"

#include <cstdlib> // For rand()

using namespace std;

Process::Process(int pid, String name, int totalLines, size_t memoryRequired)
{
	this->pid = pid;
	this->name = name;
	this->totalLines = totalLines;
	this->memoryRequired = memoryRequired;
	
	this->generateArrivalTime();

	this->commandCounter = 0;
	this->currentState = ProcessState::READY;
}

void Process::manualAddCommand(String command)
{
	String toPrint = command;
	const shared_ptr<ICommand> print = make_shared<PrintCommand>(this->pid, toPrint);
	this->commandList.push_back(print);
}

void Process::addCommand(ICommand::CommandType commandType)
{
	if (commandType == ICommand::PRINT) {
		String toPrint = "Hello, World! From Process " + to_string(this->pid); // Debug output
		const shared_ptr<ICommand> print = make_shared<PrintCommand>(this->pid, toPrint);
		this->commandList.push_back(print);
	}
}

void Process::executeCurrentCommand() const {
	this->commandList[this->commandCounter]->execute();
}

void Process::moveToNextLine()
{
    lock_guard<mutex> lock(this->processMutex); // Add a mutex to Process class
    if (this->commandCounter < this->commandList.size()) {
        this->commandCounter++;
    }
}

bool Process::isFinished() const {
	return this->commandCounter >= this->commandList.size();
}

int Process::getRemainingTime() const {
	return this->commandCounter == this->commandList.size();
}

int Process::getCommandCounter() const {
	return this->commandCounter;
}

int Process::getLinesOfCode() const {
	return this->commandList.size();
}

int Process::getPID() const {
	return this->pid;
}

int Process::getCPUCoreID() const {
	return this->cpuCoreID;
}

Process::ProcessState Process::getState() const {
	return this->currentState;
}

String Process::getName() const {
	return this->name;
}

int Process::getMemoryRequired() {
	this->memoryRequired = GlobalConfig::getInstance()->getMinMemPerProcess() + (std::rand() % (GlobalConfig::getInstance()->getMaxMemPerProcess() - GlobalConfig::getInstance()->getMinMemPerProcess() + 1));

	return this->memoryRequired;
}

tm Process::getArrivalTime() const {
	return this->localArrivalTime;
}

String Process::getFormattedArrivalTime() const
{
	char buffer[64]; // Buffer for formatted time
	strftime(buffer, sizeof(buffer), "%m/%d/%Y, %I:%M:%S %p", &localArrivalTime);
	return String(buffer);
}

void Process::generateArrivalTime()
{
	auto now = chrono::system_clock::now();
	auto currentTime = chrono::system_clock::to_time_t(now);
	localtime_s(&localArrivalTime, &currentTime); // Store the arrival time
}

void Process::generateCommands()
{
	for (int i = 0; i < this->totalLines; i++) {
		this->addCommand(ICommand::CommandType::PRINT);
	}
}

void Process::setCPUCoreID(int cpuCoreID) {
	this->cpuCoreID = cpuCoreID;
}