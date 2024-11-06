#include "SchedulerManager.h"

#include <memory>
#include <iostream>
#include <vector>

unique_ptr<Scheduler>& SchedulerManager::setScheduler(string schedulerType) {
    if (schedulerType == "\"rr\"") {
        currentScheduler = make_unique<RR_Scheduler>();
    } else if (schedulerType == "\"fcfs\"") {
        currentScheduler = make_unique<FCFS_Scheduler>();
    }
    
    return currentScheduler;
}

Scheduler& SchedulerManager::getScheduler() {
    return *currentScheduler;  // Dereference the unique_ptr to return a reference
}

void SchedulerManager::runScheduler(vector<Process>& processes, vector<CPU>& cores) {
    currentScheduler->runScheduler(processes, cores);
}

void SchedulerManager::stopScheduler() {
    currentScheduler->stopScheduler();
}

void SchedulerManager::displayProcesses() {
    currentScheduler->displayProcesses();
}

vector<Process>& SchedulerManager::getProcessList() {
    return currentScheduler->getProcessList();
}

vector<CPU>& SchedulerManager::get_cpuList() {
    return currentScheduler->get_cpuList();
}