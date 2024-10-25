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
    cout << "Starting core execution loop for core " << core.getCoreNum() << endl; // Debugging line

    while (!stopExecution) {
        {
            lock_guard<mutex> lock(processMutex); // Lock for thread safety

            if (!core.isCoreWorking() && !this->processList.empty()) {
                assignProcessToCore(core);
            }
        } // Unlock here to allow other threads to access processList

        // if (!core.isCoreWorking() && !this->processList.empty()) {
        //     assignProcessToCore(core);
        // }

        // Debug: Check if the core has a process assigned and should be working
        cout << "[coreExecutionLoop] Core " << core.getCoreNum() 
             << " working status: " << core.isCoreWorking() << endl;

        if (core.isCoreWorking()) {
            core.runProcess();

            // Debugging line
            std::cout << "Core " << core.getCoreNum() << " running process: " 
                      << core.getCpuProcess().getProcessName() 
                      << ", Instruction Count: " << core.getInstructionCount() << std::endl;

            // Process completion
            if (core.getInstructionCount() == core.getCpuProcess().getTotalInstructions()) {
                FinishedProcess finishedProcess;
                finishedProcess.process = core.getCpuProcess();
                finishedProcess.finishTime = chrono::system_clock::now();
                // finishedProcesses.push_back(finishedProcess);
                
                {
                    lock_guard<mutex> lock(processMutex); // Lock while modifying finishedProcesses
                    finishedProcesses.push_back(finishedProcess);
                }
                
                core.discardProcess();
                cout << "Process finished and discarded from core " << core.getCoreNum() << endl; // Debugging line
            }
        }
    }
}

void FCFS_Scheduler::assignProcessToCore(CPU_Core& core) {
    if (!processList.empty()) {
        core.assignProcess(processList.front());

        // Debugging line
        cout << "[assignProcessToCore] Assigned Process " << processList.front().getProcessName()
             << " to Core " << core.getCoreNum() << endl; // Logging assigned process
        
        popProcess();
    } else {
        cout << "No processes left to assign to core " << core.getCoreNum() << endl; // Debugging line
    }
}

void FCFS_Scheduler::popProcess() {
    lock_guard<mutex> lock(processMutex); // Lock while modifying processList
    processList.erase(processList.begin());
}

void FCFS_Scheduler::displayProcesses() {
    
    lock_guard<mutex> displayLock(displayMutex); // Protect display operations
    lock_guard<mutex> lock(processMutex);  // Protects access to processList and finishedProcesses

    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm local_time;

    localtime_s(&local_time, &now_time);

    // Display running processes
    cout << setfill('-') << setw(50) << "" << endl;
    cout << "Running Processes:\n";

    for (auto& core : cpuList) {    
        // Debugging line
        cout << "[Display Process] Core " << core.getCoreNum() << " working status: " << core.isCoreWorking() << endl;

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

    // Debugging line
    cout << "[Display Process]: Running Processes sucessful.\n";

    // Display finished processes
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

    // Debugging line
    cout << "[Display Process] Finished Processes sucessful.\n";

    cout << setfill('-') << setw(50) << "\n" << endl;
}

void FCFS_Scheduler::stopScheduler() {
    stopExecution = true;

    for (auto& t : this->coreThreads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

bool FCFS_Scheduler::allCoresIdle() const {
    for (const auto& core : this->cpuList) {
        if (core.isCoreWorking()) {
            return false;
        }
    }
    return true;
}