#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include "Process.h"
#include "CPU.h"
#include "Scheduler.h"

#include <thread>     
#include <chrono>     
#include <iostream> 
#include <vector>

using namespace std;

class FCFS_Scheduler : public Scheduler {
    public:
     ~FCFS_Scheduler() {}

    FCFS_Scheduler(int cpuNum, int cpuCycle, int delayPerExec, int quantum)
        : Scheduler(cpuNum, cpuCycle, delayPerExec, quantum) {}
        
    void runScheduler(vector<Process>& processes, vector<CPU>& cores) override;
    void coreExecutionLoop(CPU& core) override;
    void displayProcesses() override;
};

#endif