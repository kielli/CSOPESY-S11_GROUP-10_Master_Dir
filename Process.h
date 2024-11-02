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
        Process(const string& pName, int processId, int numInstructions);

        string executeInstruction();
        int getProcessId();
        string getProcessName();
        int getTotalInstructions();
        int getCompletedInstructions();
        int hasFinished() const;
    };

#endif