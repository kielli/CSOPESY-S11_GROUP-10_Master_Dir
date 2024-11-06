#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"
#include "CPU.h"

#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>

class Scheduler {
    public:
        virtual ~Scheduler() = default;

        virtual void runScheduler(vector<Process>& processes, vector<CPU>& cores) = 0;
        virtual void stopScheduler() = 0;

        virtual void displayProcesses() = 0;
        virtual void assignProcessToCore(CPU& core) = 0;

        virtual vector<Process>& getProcessList() = 0;
        virtual vector<CPU>& get_cpuList() = 0;
};

#endif