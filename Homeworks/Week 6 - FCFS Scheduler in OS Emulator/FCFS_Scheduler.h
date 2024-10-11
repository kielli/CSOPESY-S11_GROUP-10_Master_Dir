#include <thread>     
#include <chrono>     
#include <iostream>   

class FCFS_Scheduler {
private:
    struct FinishedProcess {
        Process process;
        std::chrono::system_clock::time_point finishTime;

        FinishedProcess() : process("", -1, 0), finishTime(std::chrono::system_clock::now()) {}
    };
    vector<Process> processList;
    vector<CPU_Core> cpuList;
    vector<FinishedProcess> finishedProcesses;
    vector<thread> coreThreads;
    bool stopExecution = false;

public:
    void runScheduler(vector<Process>& processes, vector<CPU_Core>& cores) {
        processList = processes;
        cpuList = move(cores);

        for (auto& core : cpuList) {
            coreThreads.emplace_back(&FCFS_Scheduler::coreExecutionLoop, this, ref(core));
        }
    }

    void coreExecutionLoop(CPU_Core& core) {
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


    void assignProcessToCore(CPU_Core& core) {
        if (!processList.empty()) {
            core.assignProcess(processList.front());
            popProcess();
        }
    }

    void popProcess() {
        processList.erase(processList.begin());
    }

    void displayProcesses() {
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


    void stopScheduler() {
        stopExecution = true;
        for (auto& t : coreThreads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

    bool allCoresIdle() const {
        for (const auto& core : cpuList) {
            if (core.isCoreWorking()) {
                return false;
            }
        }
        return true;
    }
};
