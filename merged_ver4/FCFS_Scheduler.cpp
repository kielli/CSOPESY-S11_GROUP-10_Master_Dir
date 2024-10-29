#include <thread>     
#include <chrono>     
#include <iostream>   
#include <sstream> 
#include <iomanip>
#include <mutex>

#include "FCFS_Scheduler.h"

void FCFS_Scheduler::runScheduler(vector<Process>& processes, vector<CPU_Core>& cores) {
    processList = processes;
    cpuList = move(cores);
    
    for (auto& core : cpuList) {
        coreThreads.emplace_back(&FCFS_Scheduler::coreExecutionLoop, this, ref(core));
    }
}

void FCFS_Scheduler::coreExecutionLoop(CPU_Core& core) {
    while (!stopExecution) {
        if (!core.isCoreWorking() && !processList.empty()) {
            // std::cout << "[DEBUG] Core " << core.getCoreNum() << " is idle, assigning new process.\n";
            // assignProcessToCore(core);
        }
        if (core.isCoreWorking()) {
            // std::cout << "[DEBUG] Core " << core.getCoreNum() << " is running process "
            //           << core.getCpuProcess().getProcessName() << ".\n";

            core.runProcess();

            if (core.getInstructionCount() == core.getCpuProcess().getTotalInstructions()) {
                FinishedProcess finishedProcess;
                finishedProcess.process = core.getCpuProcess();
                finishedProcess.finishTime = chrono::system_clock::now();
                finishedProcesses.push_back(finishedProcess);
                
                // std::cout << "[DEBUG] Process " << finishedProcess.process.getProcessName()
                //           << " completed on core " << core.getCoreNum() << ".\n";

                core.discardProcess();
            }
        }
    }
}


void FCFS_Scheduler::assignProcessToCore(CPU_Core& core) {
    if (!processList.empty()) {
        // std::cout << "[DEBUG] Assigning process " << processList.front().getProcessName()
        //           << " to core " << core.getCoreNum() << ".\n";

        core.assignProcess(processList.front());
        popProcess();
    }
}

void FCFS_Scheduler::popProcess() {
    if (!processList.empty()) {
        // std::cout << "[DEBUG] Removing process " << processList.front().getProcessName()
        //           << " from the queue.\n";

        processList.erase(processList.begin());
    }

    //processList.erase(processList.begin());
}

void FCFS_Scheduler::displayProcesses() {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm local_time;

    localtime_s(&local_time, &now_time);

    cout << setfill('-') << setw(50) << "" << endl;
    cout << "Running Processes:\n";

    for (auto& core : cpuList) {
        if (core.isCoreWorking()) {
            stringstream ss;
            ss << core.getCpuProcess().getProcessName() << " "
               << "(" << put_time(&local_time, "%m/%d/%Y %I:%M:%S %p") << ") "
               << "Core: " << core.getCoreNum() << " "
               << core.getInstructionCount() << "/" 
               << core.getCpuProcess().getTotalInstructions();
            cout << ss.str() << endl;
        }
    }

    cout << "\nFinished Processes:\n";
    
    for (auto& finished : finishedProcesses) {
        time_t finish_time = chrono::system_clock::to_time_t(finished.finishTime);
        tm finish_local_time;
        localtime_s(&finish_local_time, &finish_time);
        
        stringstream ss;
        ss << finished.process.getProcessName() << " "
           << "(" << put_time(&finish_local_time, "%m/%d/%Y %I:%M:%S %p") << ") "
           << "Finished "
           << finished.process.getTotalInstructions() << "/" 
           << finished.process.getTotalInstructions();

        cout << ss.str() << endl;
    }
    cout << setfill('-') << setw(50) << "\n" << endl;
}

void FCFS_Scheduler::stopScheduler() {
    std::cout << "[DEBUG] Stopping scheduler.\n";

    stopExecution = true;
    for (auto& t : coreThreads) {
        if (t.joinable()) {
            t.join();  // Wait for the thread to finish
            std::cout << "[DEBUG] Joined core thread.\n";
        }
    }
    std::cout << "[DEBUG] Scheduler stopped.\n";
}

bool FCFS_Scheduler::allCoresIdle() const {
    for (const auto& core : cpuList) {
        if (core.isCoreWorking()) {
            return false;
        }
    }
    return true;
}