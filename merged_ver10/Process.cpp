#include "Process.h"

#include <chrono>
#include <iomanip>

Process::Process() { }

string Process::executeInstruction() {
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	string helloMessage = "Hello World from " + pName;
	string finishMessage = "Process " + to_string(pId) + ": " + pName + " has finished";

	if (remainingI > 0) {
		remainingI--;
		return helloMessage;
	}
	else {
		return finishMessage;
	}
}

string Process::getTimestamp() const {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm localTime; //tm structure to hold the local time

    // use localtime_s for thread safety
    localtime_s(&localTime, &currentTime);

    stringstream ss;
    ss << put_time(&localTime, "%m/%d/%Y, %I:%M:%S %p");

    return ss.str();
}

string Process::getPName() {
	return this->pName;
}

int Process::getPID() {
	return this->pId;
}

int Process::getRemainingI() {
	return this->remainingI;
}

int Process::getTotalInstructions() {
	return this->numInstructions;
}

void Process::setNumInstructions(int num) {
	this->numInstructions = num;
}

void Process::setArrivalT(int arrivalT) {
	this->arrivalT = arrivalT;
}

void Process::setWaitingT(int waitingT) {
	this->waitingT = waitingT;
}

void Process::setTurnAroundT(int turnAroundT) {
	this->turnAroundT = turnAroundT;
}

int Process::getArrivalT() {
	return this->arrivalT;
}

int Process::getWaitingT() {
	return this->waitingT;
}

int Process::getTurnAroundT() {
	return this->turnAroundT;
}

int Process::hasFinished() const {
	return remainingI == 0;
}