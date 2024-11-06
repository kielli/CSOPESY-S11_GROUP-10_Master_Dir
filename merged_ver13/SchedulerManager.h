#ifndef SCHEDULERMANAGER_H
#define SCHEDULERMANAGER_H

#include "Process.h"
#include "CPU.h"
#include "FCFS_Scheduler.h"
#include "RR_Scheduler.h"

#include <memory>
#include <iostream>
#include <vector>

using namespace std;

class SchedulerManager {
    private:
        unique_ptr<Scheduler> currentScheduler;
        string scheduler;

    public:
        SchedulerManager() : currentScheduler(nullptr) {}

        // void setScheduler(string schedulerType);
        unique_ptr<Scheduler>& setScheduler(string schedulerType);
        Scheduler& getScheduler();
        void runScheduler(vector<Process>& processes, vector<CPU>& cores);

        void stopScheduler();
        void displayProcesses();

        vector<Process>& getProcessList();
        vector<CPU>& get_cpuList();
};

#endif