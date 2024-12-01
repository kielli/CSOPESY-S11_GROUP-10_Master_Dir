#include "Process.h"
#include "Process.h"
#include <cmath>

using namespace std;

Process::Process(int pid, String name, int totalLines)
{
	this->pid = pid;
	this->name = name;
	this->totalLines = totalLines;
	this->generateArrivalTime();
	this->commandCounter = 0;
	this->currentState = ProcessState::READY;
	this->memoryStatus = false;
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

void Process::initializeMemory(){
	int minProc = GlobalConfig::getInstance()->getMinMemPerProcess();
	int maxProc = GlobalConfig::getInstance()->getMaxMemPerProcess();
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(minProc, maxProc);
	int randomNum = dis(gen);

	this->memoryRequired = randomNum;
	this->framesRequired = floor(GlobalConfig::getInstance()->getMemPerFrame()/randomNum);
	//std::cout << "frames : " << this->framesRequired << std::endl;
	this->memBaseAddress = 0;
}

void Process::setMemoryStatus(bool status) {
	this->memoryStatus = status;
}

bool Process::getMemoryStatus() const{
	return this->memoryStatus;
}

void Process::moveToNextLine() {
    String sched = GlobalConfig::getInstance()->getScheduler();
    int quantum = GlobalConfig::getInstance()->getQuantumCycles();
    int elapsedTime = 0;

    if (this->commandCounter < this->commandList.size()) {
        if (sched == "rr") {
            while (elapsedTime < quantum) {
                this->commandCounter++;
                elapsedTime++;
            }
        }
        else {
            this->commandCounter++;
        }
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

tm Process::getArrivalTime() const {
	return this->localArrivalTime;
}

String Process::getFormattedArrivalTime() const {
	char buffer[64]; // Buffer for formatted time
	strftime(buffer, sizeof(buffer), "%m/%d/%Y, %I:%M:%S %p", &localArrivalTime);
	return String(buffer);
}

size_t Process::getMemBaseAddress() const {
	return this->memBaseAddress;
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

size_t Process::getMemoryRequired() const {
	return this->memoryRequired;
}

size_t Process::getFramesRequired() const {
	return this->framesRequired;
}

void Process::setMemBaseAddress(size_t address){
	this->memBaseAddress = address;
}