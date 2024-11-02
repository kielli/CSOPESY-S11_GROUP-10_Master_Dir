#ifndef CPU_CORE_H
#define CPU_CORE_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <mutex>
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
        CPU_Core(int num);
        
        void assignProcess(Process& process);
        void runProcess();    
        void discardProcess();
        mutex mtx;
        int getCoreNum() const;
        Process getCpuProcess() const;
        bool isCoreWorking() const;
        int getInstructionCount() const;
};

#endif