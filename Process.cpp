#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>          
#include <chrono>          

#include "Process.h"

Process::Process(const string& pName, int processId, int numInstructions) : processName(pName), 
        id(processId), totalInstructions(numInstructions), remainingInstructions(numInstructions) {}

string Process::executeInstruction() {
    this_thread::sleep_for(chrono::milliseconds(50));
    string helloMessage = "Hello World from " + processName;
    string finishMessage = "Process " + to_string(id) + ": " + processName + " has finished";

    if (remainingInstructions > 0) {
        remainingInstructions--;
        return helloMessage;
    } else {
        return finishMessage;
    }
}

int Process::getProcessId() {
    return this->id;
}

string Process::getProcessName() {
    return this->processName;
}

int Process::getTotalInstructions() {
    return this->totalInstructions;
}

int Process::hasFinished() const {
    return this->remainingInstructions == 0;
}