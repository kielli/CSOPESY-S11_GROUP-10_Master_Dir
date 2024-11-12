#include "RR_Scheduler.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

RR_Scheduler::RR_Scheduler() {
    this->quantum = 10;
    this->delayPerExec = 4;
    this->cpuCycle = 64;
    this->cpuNum = 4;
}

RR_Scheduler::RR_Scheduler(int cpuNum, int cpuCycle, int delayPerExec, int quantum)
    : cpuNum(cpuNum), cpuCycle(cpuCycle), delayPerExec(delayPerExec), quantum(quantum) {
    this->quantum = quantum;
    this->delayPerExec = delayPerExec;
    this->cpuCycle = cpuCycle;
    this->cpuNum = cpuNum;
}

void RR_Scheduler::runScheduler(std::vector<Process>& processes, std::vector<CPU>& cores) {
    processList = processes;
    cpuList = std::move(cores);

    for (auto& core : cpuList) {
        coreThreads.emplace_back(&RR_Scheduler::coreExecutionLoop, this, std::ref(core));
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
        memory = memoryManager.allocate(processList.front().getmemoryReq());
        if (memory != nullptr) {
            core.assignProcess(processList.front());
            processList.erase(processList.begin());
        }
        else {
            // Handle memory allocation failure if needed
            cout << "Failed to allocate memory for process: " << processList.front().getPName() << endl;
        }
        
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
    cout << memoryManager.visualizeMemory();
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
        int quantumx = 1;
        if (!core.isCoreWorking() && !processList.empty()) {
            assignProcessToCore(core);
        }

        if (core.isCoreWorking()) {
            auto startTime = chrono::steady_clock::now();
            while (!core.getCpuProcess().hasFinished()) {
                int instructionsToExecute = min(quantum, core.getCpuProcess().getRemainingI());
                core.runRProcess(instructionsToExecute);

                this_thread::sleep_for(chrono::milliseconds(delayPerExec / 100));

                auto elapsedTime = chrono::steady_clock::now() - startTime;
                if (chrono::duration_cast<chrono::seconds>(elapsedTime).count() >= quantum - delayPerExec) {
                    break;
                }
            }

            if (core.getCpuProcess().hasFinished()) {
                FinishedProcess finishedProcess;
                finishedProcess.process = core.getCpuProcess();
                finishedProcess.finishTime = chrono::system_clock::now();
                finishedProcesses.push_back(finishedProcess);
                core.discardProcess(); // Clear the finished process
                memoryManager.deallocate(memory);//deallocate process from memory
            }
            else {
                lock_guard<mutex> lock(processListMutex);
                processList.push_back(core.getCpuProcess());
                core.discardProcess();
            }
            string pname = "memory_cycle:" + to_string(quantumx);
            memoryManager.writeReportToFile(pname);
        }
    }
}


void RR_Scheduler::rotateProcessList() {
    lock_guard<mutex> lock(processListMutex);
    if (processList.empty()) return;

    Process frontProcess = processList.front();
    processList.erase(processList.begin());
    processList.push_back(frontProcess);
}
