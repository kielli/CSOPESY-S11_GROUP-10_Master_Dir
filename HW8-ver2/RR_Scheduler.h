#ifndef RR_SCHEDULER_H
#define RR_SCHEDULER_H

#include "Process.h"
#include "CPU.h"
#include "MemoryAllocator.h"
#include "FlatMemoryAllocator.h"

#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

using namespace std;

class RR_Scheduler {
    private:
        vector<Process> processList;
        vector<CPU> cpuList;
        vector<thread> coreThreads;

        int cpuNum;
        int cpuCycle;
        int delayPerExec;
        int quantum;
        bool stopExecution = false;
        void* memory;
        mutex processListMutex;

        struct FinishedProcess {
            Process process;
            chrono::system_clock::time_point finishTime;

            FinishedProcess() : process("", -1, 0, 0), finishTime(chrono::system_clock::now()) {}
        };

        vector<FinishedProcess> finishedProcesses;
        FlatMemoryAllocator memoryManager = FlatMemoryAllocator(16384);
        void coreExecutionLoop(CPU& core);
        void assignProcessToCore(CPU& core);
        void rotateProcessList();

    public:
        RR_Scheduler();
        RR_Scheduler(int cpuNum, int cpuCycle, int delayPerExec, int quantum);

        void runScheduler(vector<Process>& processes, vector<CPU>& cores);

        vector<RR_Scheduler::FinishedProcess> get_finishedProcess();
        vector<Process>& getProcessList();

        void stopScheduler();
        void displayProcesses();

        vector<CPU>& get_cpuList();
};

#endif