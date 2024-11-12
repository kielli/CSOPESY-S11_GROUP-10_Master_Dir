#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include "Process.h"
#include "CPU.h"
#include "MemoryAllocator.h"
#include "FlatMemoryAllocator.h"
#include <thread>     
#include <chrono>     
#include <iostream> 
#include <vector>

using namespace std;

class FCFS_Scheduler {
    private:
        int cpuNum;
        int cpuCycle;
        int delayPerExec;

        struct FinishedProcess {
            Process process;
            chrono::system_clock::time_point finishTime;

            FinishedProcess() : process("", -1, 0, 0), finishTime(chrono::system_clock::now()) {}
        };
        void* memory;
        FlatMemoryAllocator memoryManager = FlatMemoryAllocator(16384);
        vector<Process> processList;
        vector<CPU> cpuList;
        vector<FinishedProcess> finishedProcesses;
        vector<thread> coreThreads;

        bool stopExecution = false;

    public:
        FCFS_Scheduler();
        FCFS_Scheduler(int cpuNum, int cpuCycle, int delayPerExec);

        void runScheduler(vector<Process>& processes, vector<CPU>& cores);
        void coreExecutionLoop(CPU& core);
        void assignProcessToCore(CPU& core);
        void popProcess();
        void displayProcesses();
        void stopScheduler();
        
        bool allCoresIdle() const;

        vector<CPU>& get_cpuList();
        vector<Process>& getProcessList();
        vector<FCFS_Scheduler::FinishedProcess> get_finishedProcess();
};

#endif