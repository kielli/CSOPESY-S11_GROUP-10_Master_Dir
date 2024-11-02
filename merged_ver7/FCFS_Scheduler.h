#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include <thread>     
#include <chrono>     
#include <iostream> 
#include <vector>
#include <mutex>


#include "Processs.h"
#include "CPU.h"

using namespace std;

class FCFS_Scheduler {
private:
    int cpuNum;
    int cpuCycle;
    int delayPerExec;
    struct FinishedProcess {
        Processs process;
        std::chrono::system_clock::time_point finishTime;

        FinishedProcess() : process("", -1, 0), finishTime(std::chrono::system_clock::now()) {}
    };

    vector<Processs> processList;
    vector<CPU> cpuList;
   
    vector<FinishedProcess> finishedProcesses;
    vector<thread> coreThreads;
    bool stopExecution = false;

public:
    FCFS_Scheduler();
    FCFS_Scheduler(int cpuNum, int cpuCycle, int delayPerExec);
    void runScheduler(vector<Processs>& processes, vector<CPU>& cores);
    void coreExecutionLoop(CPU& core);
    void assignProcessToCore(CPU& core);

    void popProcess();

    void displayProcesses();
    /*void displayProcesses(std::vector<std::string>& contents);*/

    /*void displayProcesses();*/
    vector<CPU>& get_cpuList();
    vector<Processs>& getProcessList();
    void stopScheduler();
    bool allCoresIdle() const;
    vector<FCFS_Scheduler::FinishedProcess> get_finishedProcess();
};

#endif