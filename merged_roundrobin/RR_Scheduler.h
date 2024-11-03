#ifndef RR_SCHEDULER_H
#define RR_SCHEDULER_H

#include <thread>     
#include <chrono>     
#include <iostream>   
#include <algorithm>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "Process.h"
#include "CPU.h"

using namespace std;
using namespace std::chrono;

class RR_Scheduler{
private: 
    struct FinishedProcess {
        Process process;
        std::chrono::system_clock::time_point finishTime;

        FinishedProcess() : process("", -1, 0), finishTime(std::chrono::system_clock::now()) {}
    };
    int cpuNum;
    int cpuCycle;
    int quantum;
    int completed = 0;
    vector<Process> processList;
    vector<CPU> cpuList;
    vector<FinishedProcess> finishedProcesses;
    vector<thread> coreThreads;
    queue<Process> readyQueue;
    bool stopExecution = false;
    vector<Process> processQueue;
    
public:
    RR_Scheduler();
    RR_Scheduler(int Quantum);
    void runScheduler(vector<Process>& processes, vector<CPU>& cores);
    void coreExecutionLoop(CPU& core);
    void assignProcessToCore(CPU& core);

    void popProcess();
    void displayProcesses();
    vector<CPU>& get_cpuList();
    vector<Process>& getProcessList();
    void stopScheduler();
    bool allCoresIdle() const;
};

#endif