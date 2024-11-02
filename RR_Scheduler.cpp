#include <thread>     
#include <chrono>     
#include <iostream>   

#include <sstream> 
#include <iomanip>

#include "RR_Scheduler.h"

RR_Scheduler::RR_Scheduler() {
    this->quantum = 0;
    this->delay = 0;
}
RR_Scheduler::RR_Scheduler(int Quantum, int Delay) : quantum(Quantum), delay(Delay) {}



void RR_Scheduler::runScheduler(vector<Process>& processes, vector<CPU_Core>& cores) {
    processList = processes;
    cpuList = move(cores);
    steady_clock::time_point start_time = steady_clock::now();
    
    
    while (completed < processList.size()) {
        
        for (int i = 0; i < cpuList.size(); i++) {
            coreThreads.emplace_back(&RR_Scheduler::coreExecutionLoop, this, ref(cpuList[i]));
            
        }
    }
    stopExecution = true;
    /*
    for (auto& core : cpuList) {
        coreThreads.emplace_back(&RR_Scheduler::coreExecutionLoop, this, ref(core));
    }
    */
}

void RR_Scheduler::coreExecutionLoop(CPU_Core& core) {
    while (!stopExecution) {
        unique_lock<mutex> lock(core.mtx);
        if (stopExecution) break;
        
        if (!core.isCoreWorking() && !this->processList.empty()) {
            assignProcessToCore(core);
        }

        long quantum_start = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
        long quantum_end = quantum_start + quantum;
        //work for quantum time
        while (duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count() < quantum_end && core.isCoreWorking()) {
  
                core.runProcess();

                if (core.getInstructionCount() == core.getCpuProcess().getTotalInstructions()) {
                    FinishedProcess finishedProcess;
                    finishedProcess.process = core.getCpuProcess();
                    finishedProcess.finishTime = chrono::system_clock::now();
                    finishedProcesses.push_back(finishedProcess);
                    core.discardProcess();
                    completed++;
                }
                
            
        }
        //discard after quantum time
        if (core.isCoreWorking()) {
            readyQueue.push(core.getCpuProcess());
            core.discardProcess();
        }
        lock.unlock();

    }
    
}

void RR_Scheduler::assignProcessToCore(CPU_Core& core) {
    if (!readyQueue.empty()) {
        this_thread::sleep_for(milliseconds(delay));
        core.assignProcess(readyQueue.front());
        cout << "Debugging Line: Assigned " << readyQueue.front().getProcessName() << " to core " << core.getCoreNum() << endl; // Debugging line
        popProcess();
    }
}

void RR_Scheduler::popProcess() {
    processList.erase(processList.begin());
}

void RR_Scheduler::displayProcesses() {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm local_time;

    localtime_s(&local_time, &now_time);

    // Display running processes
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

            cout << "Debugging Line: Running Processes.\n"; // Debugging line
        }
    }

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

    cout << setfill('-') << setw(50) << "\n" << endl;
}

void RR_Scheduler::stopScheduler() {
    stopExecution = true;
    for (auto& t : this->coreThreads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

bool RR_Scheduler::allCoresIdle() const {
    for (const auto& core : this->cpuList) {
        if (core.isCoreWorking()) {
            return false;
        }
    }
    return true;
}