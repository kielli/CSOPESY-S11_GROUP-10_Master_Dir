#ifndef PROCESS_H
#define PROCESS_H
#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>          
#include <chrono>          

using namespace std;

class Process {
private:
    string processName;
    int id;
    int totalInstructions;
    int remainingInstructions;

public:
    Process(const string& pName, int processId, int numInstructions)
        : processName(pName), id(processId), totalInstructions(numInstructions), remainingInstructions(numInstructions) {}

    string executeInstruction() {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        string helloMessage = "Hello World from " + processName;
        string finishMessage = "Process " + to_string(id) + ": " + processName + " has finished";

        if (remainingInstructions > 0) {
            remainingInstructions--;
            return helloMessage;
        } else {
            return finishMessage;
        }
    }

    int getProcessId() {
        return id;
    }

    string getProcessName() {
        return processName;
    }

    int getTotalInstructions() {
        return totalInstructions;
    }

    int hasFinished() const {
        return remainingInstructions == 0;
    }
};
#endif
