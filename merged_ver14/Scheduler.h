#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"
#include "CPU.h"

#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>

class Scheduler
{
    protected:
        int cpuNum = 0;
        int cpuCycle = 0;
        int delayPerExec = 0;
        int quantum = 0;
        bool stopExecution = false;

        struct FinishedProcess {
            Process process;
            chrono::system_clock::time_point finishTime;

            FinishedProcess() : process("", -1, 0, static_cast<size_t>(0)), finishTime(chrono::system_clock::now()) {}
        };

        vector<Process>* processList;
        vector<CPU> cpuList;
        vector<FinishedProcess> finishedProcesses;
        vector<thread> coreThreads;

    public:
        virtual ~Scheduler() {};
        
        Scheduler(int cpuNum, int cpuCycle, int delayPerExec, int quantum)
            : cpuNum(cpuNum), cpuCycle(cpuCycle), delayPerExec(delayPerExec), quantum(quantum) {

            // Loop to initialize `cpuList` with CPU objects
            for (int i = 0; i < this->cpuNum; i++) {
                this->cpuList.push_back(CPU(i));  // Assuming CPU(int id) constructor exists
            }
        }

        virtual void runScheduler(vector<Process>& processes, vector<CPU>& cores) = 0;
        virtual void stopScheduler() = 0;
        virtual void assignProcessToCore(CPU& core) = 0;
        virtual vector<CPU>& get_cpuList() = 0;
        virtual vector<Process>& getProcessList() = 0;
        virtual void coreExecutionLoop(CPU& core) = 0;
        virtual void displayProcesses() = 0;

        void popProcess();
        bool allCoresIdle() const;
        vector<Scheduler::FinishedProcess> get_finishedProcess();
};

#endif