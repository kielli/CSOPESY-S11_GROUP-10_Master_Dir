#ifndef CPU_H
#define CPU_H

#include "Process.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <iomanip>

using namespace std;

class CPU {
private:
    int coreNum;
    bool isWorking = false;
    Process cpuProcess;
    int instructionCount;
    ofstream outFile;

public:
    CPU(int num) : coreNum(num), cpuProcess("null", -1, 0,0) {}

    CPU(const CPU& other)
        : coreNum(other.coreNum), cpuProcess(other.cpuProcess) {
    }

    void assignProcess(Process& process);
    void runProcess();
    void runRProcess(int instructionLimit);
    void runRRProcess();
    void discardProcess();

    int getCoreNum() const;
    int getInstructionCount() const;

    Process getCpuProcess();

    bool isCoreWorking() const;
};

#endif