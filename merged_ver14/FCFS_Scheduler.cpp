#include "FCFS_Scheduler.h"

#include <thread>     
#include <chrono>     
#include <iostream>   
#include <sstream> 
#include <iomanip>

FCFS_Scheduler::FCFS_Scheduler() { }

FCFS_Scheduler::FCFS_Scheduler(int cpuNum, int cpuCycle, int delayPerExec) {
	this->cpuNum = cpuNum;
	this->cpuCycle = cpuCycle;
	this->delayPerExec = delayPerExec;
}

void FCFS_Scheduler::runScheduler(vector<Process>& processes, vector<CPU>& cores) {
    this->processList = processes;
    cpuList = move(cores);

    for (auto& core : cpuList) {
        coreThreads.emplace_back(&FCFS_Scheduler::coreExecutionLoop, this, ref(core));
    }
}

void FCFS_Scheduler::coreExecutionLoop(CPU& core) {
    while (!stopExecution) {
        if (!core.isCoreWorking() && !processList.empty()) {
            assignProcessToCore(core);
        }
        if (core.isCoreWorking()) {
            core.runProcess();

            if (core.getInstructionCount() == core.getCpuProcess().getTotalInstructions()) {
                FinishedProcess finishedProcess;
                finishedProcess.process = core.getCpuProcess();
                finishedProcess.finishTime = chrono::system_clock::now();
                finishedProcesses.push_back(finishedProcess);
                core.discardProcess();
            }
        }
    }
}

void FCFS_Scheduler::assignProcessToCore(CPU& core) {
    if (!this->processList.empty()) {
        core.assignProcess(this->processList.front());
        popProcess();
    }
}

void FCFS_Scheduler::popProcess() {
    processList.erase(processList.begin());
}

string FCFS_Scheduler::displayProcesses() {
    stringstream output;

    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm local_time;

    int cpuUtilization = 0;
    int usedCores = 0;
    int totalCores = cpuList.size();

    localtime_s(&local_time, &now_time);

    // Count  of used cores
    for (auto& core : cpuList) {
        if (core.isCoreWorking()) {
            usedCores++;
        }
    }

    // CPU Utilization calculation
    if (totalCores > 0)
        cpuUtilization = (usedCores * 100) / totalCores;
    else
        cpuUtilization = 0;

    output << "\nCPU Utilization: " << cpuUtilization << "%\n";
    output << "Cores used: " << usedCores << "\n";
    output << "Cores available: " << (totalCores - usedCores) << "\n";
    
    output << setfill('-') << setw(50) << "" << endl;

    output << "Running Processes:\n";
    for (auto& core : cpuList) {
        if (core.isCoreWorking()) {
            stringstream processInfo;

            processInfo << core.getCpuProcess().getPName() << " "
                        << "(" << put_time(&local_time, "%m/%d/%Y %I:%M:%S %p") << ") "
                        << "Core: " << core.getCoreNum() << " "
                        << core.getInstructionCount() << "/"
                        << core.getCpuProcess().getTotalInstructions();

            output << processInfo.str() << endl;
        }
    }

    output << "\nFinished Processes:\n";
    for (auto& finished : finishedProcesses) {
        time_t finish_time = chrono::system_clock::to_time_t(finished.finishTime);
        tm finish_local_time;
        localtime_s(&finish_local_time, &finish_time);

        stringstream finishedProcessInfo;

        finishedProcessInfo << finished.process.getPName() << " "
                            << "(" << put_time(&finish_local_time, "%m/%d/%Y %I:%M:%S %p") << ") "
                            << "Finished "
                            << finished.process.getTotalInstructions() << "/"
                            << finished.process.getTotalInstructions();

        output << finishedProcessInfo.str() << endl;
    }

    output << setfill('-') << setw(50) << "\n" << endl;

    return output.str();
}

vector<CPU>& FCFS_Scheduler::get_cpuList() {
    return this->cpuList;
}

vector<Process>& FCFS_Scheduler::getProcessList() {
    return this->processList;
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

vector<FCFS_Scheduler::FinishedProcess> FCFS_Scheduler::get_finishedProcess() {
    return this->finishedProcesses;
}