#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "Process.cpp"

using namespace std;

class FCFS_Scheduler {
private:
    int numCores;
    vector<vector<Process>> processQueues;

public:
    FCFS_Scheduler(int cores) : numCores(cores), processQueues(cores) {}

    void addProcess(const Process& process, int core = 0){
        if(core>=0 && core<numCores){
            processQueues[core].push_back(process);
        } else {
            cerr << "Invalid core specified for process addition";
        }
    }

    void sortProcessQueues() {
        for(auto& queue : processQueues){
            std::sort(queue.begin(), queue.end(), [](Process& a, Process& b){
                return a.getRemainingInstructions() > b.getRemainingInstructions();
            });
        }
    }

    void runScheduler() {
        while(!processQueues[0].empty()){
            for(int core = 0; core < numCores; core++){
                if(!processQueues[core].empty()){
                    Process currentProcess = processQueues[core].back();
                    processQueues[core].pop_back();

                    while (!currentProcess.hasFinished()){
                        currentProcess.executeInstruction();
                    }

                    cout << "Process " << currentProcess.getRemainingInstructions() << " completed on Core " << core << endl;
                }
            }
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    vector<Process> processes;
    processes.emplace_back("Process_1", 1, 10);
    processes.emplace_back("Process_2", 2, 8);
    processes.emplace_back("Process_3", 3, 15);

    FCFS_Scheduler fcfsscheduler(3);

    for(const auto& process : processes){
        fcfsscheduler.addProcess(process);
    }

    fcfsscheduler.sortProcessQueues();

    fcfsscheduler.runScheduler();

    return 0;
}