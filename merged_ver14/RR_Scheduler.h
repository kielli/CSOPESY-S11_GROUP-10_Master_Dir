#ifndef RR_SCHEDULER_H
#define RR_SCHEDULER_H

#include "Process.h"
#include "CPU.h"
#include "Scheduler.h"

#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

using namespace std;

class RR_Scheduler : public Scheduler {
    private:
        vector<Process> processList;
        vector<CPU> cpuList;
        vector<thread> coreThreads;

        int cpuNum;
        int cpuCycle;
        int delayPerExec;
        int quantum;
        bool stopExecution = false;

        mutex processListMutex;

        struct FinishedProcess {
            Process process;
            chrono::system_clock::time_point finishTime;

            FinishedProcess() : process("", -1, 0, 0), finishTime(chrono::system_clock::now()) {}
        };

        vector<FinishedProcess> finishedProcesses;

        void coreExecutionLoop(CPU& core);
        void assignProcessToCore(CPU& core) override;
        void rotateProcessList();

    public:
        ~RR_Scheduler(){}

        RR_Scheduler (int cpuNum, int cpuCycle, int delayPerExec, int quantum)
            : Scheduler(cpuNum, cpuCycle, delayPerExec, quantum) {}

        // Delete the copy constructor and assignment operator to prevent copying
        RR_Scheduler(const RR_Scheduler&) = delete;
        RR_Scheduler& operator=(const RR_Scheduler&) = delete;

        void runScheduler(vector<Process>& processes, vector<CPU>& cores) override;

        vector<RR_Scheduler::FinishedProcess> get_finishedProcess();
        vector<Process>& getProcessList() override;
        vector<CPU>& get_cpuList() override;

        void stopScheduler() override;
        void displayProcesses() override;
};

#endif