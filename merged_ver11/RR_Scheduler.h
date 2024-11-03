#ifndef RR_SCHEDULER_H
#define RR_SCHEDULER_H

#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include "Processs.h"
#include "CPU.h"

class RR_Scheduler {
private:
    std::vector<Processs> processList;
    std::vector<CPU> cpuList;
    std::vector<std::thread> coreThreads;
    int cpuNum;
    int cpuCycle;
    int delayPerExec;
    int quantum;
    bool stopExecution = false;
    std::mutex processListMutex;

    struct FinishedProcess {
        Processs process;
        std::chrono::system_clock::time_point finishTime;

        FinishedProcess() : process("", -1, 0), finishTime(std::chrono::system_clock::now()) {}
    };
    vector<FinishedProcess> finishedProcesses;
    void coreExecutionLoop(CPU& core);
    void assignProcessToCore(CPU& core);
    void rotateProcessList();

public:
    RR_Scheduler();
    RR_Scheduler(int cpuNum, int cpuCycle, int delayPerExec, int quantum);

    void runScheduler(std::vector<Processs>& processes, std::vector<CPU>& cores);
    vector<RR_Scheduler::FinishedProcess> get_finishedProcess();
    vector<Processs>& getProcessList();
    void stopScheduler();
    void displayProcesses();
    vector<CPU>& get_cpuList();

};

#endif // RR_SCHEDULER_H
