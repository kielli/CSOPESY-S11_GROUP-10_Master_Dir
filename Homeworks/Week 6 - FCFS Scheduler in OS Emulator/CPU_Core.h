#ifndef CPU_CORE_H
#define CPU_CORE_H
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include "Process.h"

using namespace std;

class CPU_Core {
private:
    int coreNum;
    bool isWorking = false;
    Process cpuProcess;
    int instructionCount;
    ofstream outFile;

public:
    CPU_Core(int num) : coreNum(num), cpuProcess("null", -1, 0) {}

    void assignProcess(Process& process) {
        cpuProcess = process; 
        instructionCount = 0;
        isWorking = true;
    }

    void runProcess() {
    outFile.open("process_" + to_string(cpuProcess.getProcessId()) + ".txt");
    if (!outFile.is_open()) {
        cerr << "Failed to open the log file for process: " << cpuProcess.getProcessId() << endl;
        return;
    }

    outFile << "Process Name: " << cpuProcess.getProcessName() << "\nLogs:\n\n";

    while (!cpuProcess.hasFinished() && cpuProcess.getProcessId() != -1) {
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        tm local_time;
        localtime_s(&local_time, &now_time);

        stringstream logMessage;
        logMessage << "(" << put_time(&local_time, "%m/%d/%Y %I:%M:%S %p") << ") "
                   << "Core: " << coreNum << " "
                   << "\"" << cpuProcess.executeInstruction() << "\"";

        outFile << logMessage.str() << endl;

        instructionCount++;
    }

    outFile.close();
}

    void discardProcess() {
        isWorking = false;
    }

    int getCoreNum() const {
        return coreNum;
    }

    Process getCpuProcess() const {
        return cpuProcess;
    }

    bool isCoreWorking() const {
        return isWorking;
    }

    int getInstructionCount() const {
        return instructionCount;
    }
};
#endif