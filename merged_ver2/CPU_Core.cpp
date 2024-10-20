#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <sstream> 
#include <iomanip>

#include "Process.h"
#include "CPU_Core.h"

CPU_Core::CPU_Core(int num) : coreNum(num), cpuProcess("null", -1, 0) {}

void CPU_Core::assignProcess(Process& process) {
    cpuProcess = process; 
    instructionCount = 0;
    isWorking = true;
}

void CPU_Core::runProcess() {
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
                    << "Core: " << this->coreNum << " "
                    << "\"" << cpuProcess.executeInstruction() << "\"";

            outFile << logMessage.str() << endl;

            instructionCount++;
        }

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