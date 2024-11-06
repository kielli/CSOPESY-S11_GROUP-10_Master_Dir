#include "SchedulerManager.h"

#include <memory>
#include <iostream>
#include <vector>

// void SchedulerManager::setScheduler(string schedulerType) {
//     if (schedulerType == "\"rr\"") {
//         currentScheduler = make_unique<RR_Scheduler>();
//     } else if (schedulerType == "\"fcfs\"") {
//         currentScheduler = make_unique<FCFS_Scheduler>();
//     }
// }

unique_ptr<Scheduler>& SchedulerManager::setScheduler(string schedulerType) {
    if (schedulerType == "\"rr\"") {
        currentScheduler = make_unique<RR_Scheduler>();
    } else if (schedulerType == "\"fcfs\"") {
        currentScheduler = make_unique<FCFS_Scheduler>();
    }
    
    return currentScheduler;
}

Scheduler& SchedulerManager::getScheduler() {
    if (currentScheduler) {
        return *currentScheduler;  // Dereference the unique_ptr to return a reference
    } else {
        throw std::runtime_error("No scheduler is set");
    }
}

void SchedulerManager::runScheduler(vector<Process>& processes, vector<CPU>& cores) {
    if (currentScheduler) {
        currentScheduler->runScheduler(processes, cores);
    } else {
        cerr << "No scheduler selected!" << endl;
    }
}

void SchedulerManager::stopScheduler() {
    if (currentScheduler) {
        currentScheduler->stopScheduler();
    }
}

void SchedulerManager::displayProcesses() {
    if (currentScheduler) {
        currentScheduler->displayProcesses();
    }
}

vector<Process>& SchedulerManager::getProcessList() {
    if (currentScheduler) {
        return currentScheduler->getProcessList();
    } else {
        static vector<Process> empty;
        return empty;
    }
}

vector<CPU>& SchedulerManager::get_cpuList() {
    if (currentScheduler) {
        return currentScheduler->get_cpuList();
    } else {
        static vector<CPU> empty;
        return empty;
    }
}