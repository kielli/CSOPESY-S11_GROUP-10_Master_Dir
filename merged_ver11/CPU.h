#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <iomanip>

#include "Processs.h"

using namespace std;

class CPU{
private:
    int coreNum;
    bool isWorking = false;
    Processs cpuProcess;
    int instructionCount;
    ofstream outFile;
   /* vector<Processs> readyQue;
    vector<Processs> finished;*/

public:
    CPU(int num) : coreNum(num), cpuProcess("null", -1, 0) {}

    CPU(const CPU& other)
        : coreNum(other.coreNum), cpuProcess(other.cpuProcess) {
    }

   /* CPU(){}
    CPU(const CPU&) = delete;
    CPU& operator=(const CPU&) = delete;
    CPU(int num);*/

   /* void runProcess();
    void discardProcess();
    void assignProcess(Processs process);

    int getCoreNum() const;
    Processs getCpuProcess() const;
    bool isCoreWorking() const;
    vector<Processs> getfinished();
    vector<Processs> getQue();*/

    void assignProcess(Processs& process);

    void runProcess();

    void runRProcess(int instructionLimit);

    void runRRProcess();

    void discardProcess();

    int getCoreNum() const;

    Processs getCpuProcess();

    bool isCoreWorking() const;

    int getInstructionCount() const;

};

#endif