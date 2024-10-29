#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <sstream> 
#include <iomanip>
#include <mutex>

#include "Process.h"
#include "CPU_Core.h"

CPU_Core::CPU_Core(int num) : coreNum(num), cpuProcess("null", -1, 0) {}

void CPU_Core::assignProcess(Process& process) {
    cpuProcess = process; 
    instructionCount = 0;
    isWorking = true;
}

void CPU_Core::runProcess() {
    // Debug line to indicate process start
    cout << "Debug: Starting execution of process " << cpuProcess.getProcessId() 
         << " (" << cpuProcess.getProcessName() << ") on Core " << getCoreNum() << endl;

    outFile.open("process_" + to_string(cpuProcess.getProcessId()) + ".txt");
    if (!outFile.is_open()) {
        cerr << "Failed to open the log file for process: " << cpuProcess.getProcessId() << endl;
        return;
    }

    outFile << "Process Name: " << cpuProcess.getProcessName() << "\nLogs:\n\n";

    while (!cpuProcess.hasFinished() && cpuProcess.getProcessId() != -1) {
        string instructionLog = cpuProcess.executeInstruction();
        outFile << instructionLog << endl;
        instructionCount++;
    }

    // Debug line to indicate process completion
    // cout << "Debug: Finished execution of process " << cpuProcess.getProcessId() 
    //      << " on Core " << getCoreNum() << endl;

    outFile.close();
}

void CPU_Core::discardProcess() {
    this->isWorking = false;
}

int CPU_Core::getCoreNum() const {
    return this->coreNum;
}

Process CPU_Core::getCpuProcess() const {
    return this->cpuProcess;
}

bool CPU_Core::isCoreWorking() const {
    return this->isWorking;
}

int CPU_Core::getInstructionCount() const {
    return this->instructionCount;
}