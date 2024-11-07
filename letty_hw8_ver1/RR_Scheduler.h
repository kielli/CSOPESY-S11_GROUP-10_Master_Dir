#ifndef RR_SCHEDULER_H
#define RR_SCHEDULER_H

#include "MemoryAllocator.h"
#include "FlatMemAllocatorRR.h"
#include "Process.h"
#include "CPU.h"

#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <iostream>

using namespace std;

class RR_Scheduler {
private:
    vector<Process> processList;
    vector<CPU> cpuList;
    vector<thread> coreThreads;
    MemoryAllocator& memoryAllocator;

    int cpuNum=0;
    int cpuCycle=0;
    int delayPerExec=0;
    int quantum=0;
    bool stopExecution = false;

   /* mutex processListMutex;*/

    struct FinishedProcess {
        Process process;
        chrono::system_clock::time_point finishTime;

        FinishedProcess() : process("", -1, 0, static_cast<size_t>(0)), finishTime(chrono::system_clock::now()) {}
    };

    vector<FinishedProcess> finishedProcesses;

    void storeMemoryPtr(void* memory);

    void* getMemoryPtr(void* memory);

    void coreExecutionLoop(CPU& core);
    /*void assignProcessToCore(CPU& core);*/
    void rotateProcessList();

public:
    void* memory;
    ~RR_Scheduler() = default;
    RR_Scheduler(MemoryAllocator& memormyAllocator) : memoryAllocator(memoryAllocator) {}
    RR_Scheduler(int cpuNum, int cpuCycle, int delayPerExec, int quantum, MemoryAllocator& memormyAllocator);

    void runScheduler(vector<Process>& processes, vector<CPU>& cores);

    vector<RR_Scheduler::FinishedProcess> get_finishedProcess();
    vector<Process>& getProcessList();

    void stopScheduler();
    void displayProcesses();

    vector<CPU>& get_cpuList();
};

#endif