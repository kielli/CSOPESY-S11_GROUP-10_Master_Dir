#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include <thread>     
#include <chrono>     
#include <iostream>   
#include <vector>
#include "Process.h"
#include "CPU_Core.h"

class FCFS_Scheduler {
    private:
        struct FinishedProcess {
            Process process;
            std::chrono::system_clock::time_point finishTime;

            FinishedProcess() : process("", -1, 0), finishTime(std::chrono::system_clock::now()) {}
        };

        vector<Process> processList;
        vector<CPU_Core> cpuList;
        vector<FinishedProcess> finishedProcesses;
        vector<thread> coreThreads;
        
        bool stopExecution = false;

    public:
        void runScheduler(vector<Process>& processes, vector<CPU_Core>& cores);
        void coreExecutionLoop(CPU_Core& core);
        void assignProcessToCore(CPU_Core& core);

        void popProcess();
        void displayProcesses();

        void stopScheduler();
        bool allCoresIdle() const;
};

#endif