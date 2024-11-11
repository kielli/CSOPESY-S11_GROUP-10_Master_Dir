#include "RR_Scheduler.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

void RR_Scheduler::runScheduler(vector<Process>& processes, vector<CPU>& cores) {
    processList = processes;
    cpuList = std::move(cores);

    for (auto& core : cpuList) {
        coreThreads.emplace_back(&RR_Scheduler::coreExecutionLoop, this, ref(core));
    }
}

vector<RR_Scheduler::FinishedProcess> RR_Scheduler::get_finishedProcess() {
    return this->finishedProcesses;
}

vector<Process>& RR_Scheduler::getProcessList() {
    return this->processList;
}

void RR_Scheduler::assignProcessToCore(CPU& core) {
    lock_guard<mutex> lock(processListMutex);

    if (!processList.empty()) {
        core.assignProcess(processList.front());
        processList.erase(processList.begin());
    }
}

void RR_Scheduler::stopScheduler() {
    stopExecution = true;
    for (auto& t : coreThreads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void RR_Scheduler::displayProcesses() {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm local_time;

    int cpuUtilization = 0;
    int usedCores = 0;
    int totalCores = cpuList.size();

    localtime_s(&local_time, &now_time);

    // Count used cores
    for (const auto& core : cpuList) {
        if (core.isCoreWorking()) {
            usedCores++;
        }
    }

    // Calculate CPU Utilization
    if (totalCores > 0)
        cpuUtilization = (usedCores * 100) / totalCores;
    else
        cpuUtilization = 0;

    cout << "\nCPU Utilization: " << cpuUtilization << "%\n";
    cout << "Cores used: " << usedCores << "\n";
    cout << "Cores available: " << (totalCores - usedCores) << "\n\n";

    cout << setfill('-') << setw(50) << "" << endl;
    cout << "Running Processes:\n";
    for (auto& core : cpuList) {
        if (core.isCoreWorking()) {
            stringstream ss;
            
            ss << core.getCpuProcess().getPName() << " "
               << "(" << put_time(&local_time, "%m/%d/%Y %I:%M:%S %p") << ") "
               << "Core: " << core.getCoreNum() << " "
               << core.getCpuProcess().getTotalInstructions() - core.getCpuProcess().getRemainingI() << "/"
               << core.getCpuProcess().getTotalInstructions();
            cout << ss.str() << std::endl;
        }
    }

    cout << "\nFinished Processes:\n";
    for (auto& finished : finishedProcesses) {
        time_t finish_time = chrono::system_clock::to_time_t(finished.finishTime);
        tm finish_local_time;
        localtime_s(&finish_local_time, &finish_time);

        stringstream ss;
        ss << finished.process.getPName() << " "
            << "(" << put_time(&finish_local_time, "%m/%d/%Y %I:%M:%S %p") << ") "
            << "Finished "
            << finished.process.getTotalInstructions() << "/"
            << finished.process.getTotalInstructions();

        cout << ss.str() << endl;
    }
    cout << setfill('-') << setw(50) << "\n" << std::endl;
}

vector<CPU>& RR_Scheduler::get_cpuList() {
    return this->cpuList;
}

void RR_Scheduler::coreExecutionLoop(CPU& core) {
    while (!stopExecution) {
        // Assign a process to the core if it's not currently working and there are processes in the queue
        if (!core.isCoreWorking() && !processList.empty()) {
            assignProcessToCore(core);
        }

        // If the core is currently working, execute the process
        if (core.isCoreWorking()) {
            auto startTime = std::chrono::steady_clock::now(); // Start time tracking

            // Execute the current process for the specified quantum time (in seconds)
            while (!core.getCpuProcess().hasFinished()) {
                int instructionsToExecute = min(quantum, core.getCpuProcess().getRemainingI());
                core.runRProcess(instructionsToExecute); // Executes one instruction

                // Sleep for a short time to allow other processes to get CPU time
                //std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Adjust as necessary
                this_thread::sleep_for(chrono::seconds(delayPerExec/100));
                // Check the elapsed time
                auto currentTime = chrono::steady_clock::now();
                auto elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - startTime);

                // If we've run for the specified quantum time, break
                if (elapsedTime.count() >= quantum-delayPerExec) {
                    break;
                }
            }

            // After the quantum time, check if the process finished
            if (core.getCpuProcess().hasFinished()) {
                FinishedProcess finishedProcess;
                finishedProcess.process = core.getCpuProcess();
                finishedProcess.finishTime = chrono::system_clock::now();
                finishedProcesses.push_back(finishedProcess);
                core.discardProcess(); // Clear the finished process
            }
            else {
                // If the process did not finish, requeue it
                lock_guard<mutex> lock(processListMutex);
                processList.push_back(core.getCpuProcess()); // Requeue the process
                core.discardProcess(); // Clear the core for the next process
            }
        }
    }
}

void RR_Scheduler::rotateProcessList() {
    lock_guard<mutex> lock(processListMutex);
    
    if (!processList.empty()) {
        Process& frontProcess = processList.front();
        processList.erase(processList.begin());
        processList.push_back(frontProcess);
    }
}