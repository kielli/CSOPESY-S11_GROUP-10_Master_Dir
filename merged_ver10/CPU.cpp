#include "CPU.h"

void CPU::assignProcess(Process& process) {
    cpuProcess = process;
    instructionCount = 0;
    isWorking = true;
}

void CPU::runProcess() {
    outFile.open("process_" + to_string(cpuProcess.getPID()) + ".txt");
    if (!outFile.is_open()) {
        cerr << "Failed to open the log file for process: " << cpuProcess.getPID() << endl;
        return;
    }

    outFile << "Process Name: " << cpuProcess.getPName() << "\nLogs:\n\n";

    while (!cpuProcess.hasFinished() && cpuProcess.getPID() != -1) {
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

void CPU::discardProcess() {
    isWorking = false;
}

int CPU::getCoreNum() const {
    return coreNum;
}

Process CPU::getCpuProcess() const {
    return cpuProcess;
}

bool CPU::isCoreWorking() const {
    return isWorking;
}

int CPU::getInstructionCount() const {
    return instructionCount;
}