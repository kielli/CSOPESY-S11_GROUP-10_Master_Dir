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
        mutex processListMutex;

    public:
        ~RR_Scheduler() {}

        RR_Scheduler(int cpuNum, int cpuCycle, int delayPerExec, int quantum)
            : Scheduler(cpuNum, cpuCycle, delayPerExec, quantum) {}

        void coreExecutionLoop(CPU& core) override;
        void runScheduler(vector<Process>& processes, vector<CPU>& cores) override;
        void displayProcesses() override;
};

#endif