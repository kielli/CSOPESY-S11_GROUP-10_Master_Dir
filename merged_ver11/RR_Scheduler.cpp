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

void RR_Scheduler::runScheduler(std::vector<Processs>& processes, std::vector<CPU>& cores) {
    processList = processes;
    cpuList = std::move(cores);

    for (auto& core : cpuList) {
        coreThreads.emplace_back(&RR_Scheduler::coreExecutionLoop, this, std::ref(core));
    }
}

vector<RR_Scheduler::FinishedProcess> RR_Scheduler::get_finishedProcess() {
    return this->finishedProcesses;
}

vector<Processs>& RR_Scheduler::getProcessList() {
    return this->processList;
}

void RR_Scheduler::assignProcessToCore(CPU& core) {
    std::lock_guard<std::mutex> lock(processListMutex);
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
    auto now = std::chrono::system_clock::now();
    time_t now_time = std::chrono::system_clock::to_time_t(now);
    tm local_time;

    localtime_s(&local_time, &now_time);
    std::cout << std::setfill('-') << std::setw(50) << "" << std::endl;
    std::cout << "Running Processes:\n";
    for (auto& core : cpuList) {
        if (core.isCoreWorking()) {
            std::stringstream ss;
            ss << core.getCpuProcess().getPName() << " "
               << "(" << std::put_time(&local_time, "%m/%d/%Y %I:%M:%S %p") << ") "
               << "Core: " << core.getCoreNum() << " "
               << core.getCpuProcess().getTotalInstructions() - core.getCpuProcess().getRemainingI() << "/"
               << core.getCpuProcess().getTotalInstructions();
            std::cout << ss.str() << std::endl;
        }
    }

    std::cout << "\nProcesses in Queue:\n";
    for (auto& process : processList) {
        std::cout << process.getPName() << " with remaining instructions: " << process.getRemainingI() << "\n";
    }
    std::cout << std::setfill('-') << std::setw(50) << "\n" << std::endl;
}

vector<CPU>& RR_Scheduler::get_cpuList()
{
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
                int instructionsToExecute = std::min(quantum, core.getCpuProcess().getRemainingI());
                core.runRProcess(instructionsToExecute); // Executes one instruction

                // Sleep for a short time to allow other processes to get CPU time
                //std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Adjust as necessary
                std::this_thread::sleep_for(std::chrono::seconds(delayPerExec/100));
                // Check the elapsed time
                auto currentTime = std::chrono::steady_clock::now();
                auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);

                // If we've run for the specified quantum time, break
                if (elapsedTime.count() >= quantum-delayPerExec) {
                    break;
                }
            }

            // After the quantum time, check if the process finished
            if (core.getCpuProcess().hasFinished()) {
                FinishedProcess finishedProcess;
                finishedProcess.process = core.getCpuProcess();
                finishedProcess.finishTime = std::chrono::system_clock::now();
                finishedProcesses.push_back(finishedProcess);
                core.discardProcess(); // Clear the finished process
            }
            else {
                // If the process did not finish, requeue it
                std::lock_guard<std::mutex> lock(processListMutex);
                processList.push_back(core.getCpuProcess()); // Requeue the process
                core.discardProcess(); // Clear the core for the next process
            }
        }
    }
}

void RR_Scheduler::rotateProcessList() {
    std::lock_guard<std::mutex> lock(processListMutex);
    if (!processList.empty()) {
        Processs& frontProcess = processList.front();
        processList.erase(processList.begin());
        processList.push_back(frontProcess);
    }
}
