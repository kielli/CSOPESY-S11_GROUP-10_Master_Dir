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
    private:
        int cpuNum;
        int cpuCycle;
        int delayPerExec;

        struct FinishedProcess {
            Process process;
            chrono::system_clock::time_point finishTime;

            FinishedProcess() : process("", -1, 0, 0), finishTime(chrono::system_clock::now()) {}
        };

        vector<Process> processList;
        vector<CPU> cpuList;
        vector<FinishedProcess> finishedProcesses;
        vector<thread> coreThreads;

        bool stopExecution = false;

    public:
        // FCFS_Scheduler();
        // FCFS_Scheduler(int cpuNum, int cpuCycle, int delayPerExec);

         ~FCFS_Scheduler() {}
         
        FCFS_Scheduler(int cpuNum, int cpuCycle, int delayPerExec, int quantum) 
            : Scheduler(cpuNum, cpuCycle, delayPerExec, quantum)  {}

        void runScheduler(vector<Process>& processes, vector<CPU>& cores) override;
        void stopScheduler() override;

        void coreExecutionLoop(CPU& core);
        void assignProcessToCore(CPU& core) override;
        
        void popProcess();
        void displayProcesses() override;
        
        bool allCoresIdle() const;

        vector<CPU>& get_cpuList() override;
        vector<Process>& getProcessList() override;

        vector<FCFS_Scheduler::FinishedProcess> get_finishedProcess();
};

#endif