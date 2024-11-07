#include "CPU.h"

void CPU::assignProcess(Process& process) {
    cpuProcess = process;
    instructionCount = 0;
    isWorking = true;
}

void CPU::runProcess() {
    outFile.open("process" + to_string(cpuProcess.getPID()) + ".txt");

    if (!outFile.is_open()) {
        cerr << "Failed to open the log file for process: " << cpuProcess.getPID() << endl;
        return;
    }

    outFile << "\nProcess Name: " << cpuProcess.getPName() << "\nLogs:\n\n";

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

void CPU::runRProcess(int instructionLimit) {
    outFile.open("process_" + to_string(cpuProcess.getPID()) + ".txt", std::ios_base::app);
    if (!outFile.is_open()) {
        cerr << "Failed to open the log file for process: " << cpuProcess.getPID() << endl;
        return;
    }

    outFile << "Process Name: " << cpuProcess.getPName() << "\nLogs:\n\n";

    for (int i = 0; i < instructionLimit && !cpuProcess.hasFinished(); ++i) {
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        tm local_time;
        localtime_s(&local_time, &now_time);

        stringstream logMessage;
        logMessage << "(" << put_time(&local_time, "%m/%d/%Y %I:%M:%S %p") << ") "
            << "Core: " << coreNum << " "
            << "\"" << cpuProcess.executeInstruction() << "\"";

        outFile << logMessage.str() << endl;
    }

    outFile.close();
}

void CPU::runRRProcess() {
    outFile.open("process_" + to_string(cpuProcess.getPID()) + ".txt", std::ios_base::app);
    if (!outFile.is_open()) {
        cerr << "Failed to open the log file for process: " << cpuProcess.getPID() << endl;
        return;
    }

    outFile << "\nProcess Name: " << cpuProcess.getPName() << "\nLogs:\n\n";

    if (!cpuProcess.hasFinished() && cpuProcess.getPID() != -1) {
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

Process CPU::getCpuProcess() {
    return cpuProcess;
}

bool CPU::isCoreWorking() const {
    return isWorking;
}

int CPU::getInstructionCount() const {
    return instructionCount;
}