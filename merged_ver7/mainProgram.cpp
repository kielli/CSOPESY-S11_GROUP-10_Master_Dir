#include "ScreenManager.h"

//for vscode, uncomment
//#include "ScreenManager.cpp"
//#include "Screen.cpp"
//#include "CreatedScreen.cpp"
//#include "MainScreen.cpp"

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

ScreenManager screenManager;
thread schedulerThread;

//void runSchedulerFCFS() {
//    auto& schedulerMain = screenManager.getScheduler();
//    auto& processList = screenManager.getProcessList();
//    auto& cpuList = screenManager.getScheduler().get_cpuList();
//
//   schedulerThread = thread([&schedulerMain, &processList, &cpuList]() {
//        schedulerMain.runScheduler(processList, cpuList);
//        });
//}

struct Config {
    int numCPU;
    string scheduler;
    int quantumCycles;
    int batchProcessFreq;
    int minInstructions;
    int maxInstructions;
    int delayPerExec;
};

Config initialize() {
    Config config; 
    cout << "number of CPU: ";
    cin >> config.numCPU;
    cin.ignore();

    cout << "Scheduler type: ";
    getline(cin, config.scheduler);

    cout << "Quantum cycles: ";
    cin >> config.quantumCycles;

    cout << "Batch process frequency: ";
    cin >> config.batchProcessFreq;

    cout << "Min instructions: ";
    cin >> config.minInstructions;

    cout << "Max instructions: ";
    cin >> config.maxInstructions;

    cout << "Delay per Execution: ";
    cin >> config.delayPerExec;

    return config;
}

string printConfig(const Config& config) {
    stringstream ss;
    ss << "\nConfiguration:\n";
    ss << "numCPU: " << config.numCPU << "\nScheduler: " << config.scheduler
        << "\nQuantum cycles: " << config.quantumCycles
        << "\nBatch process frequency: " << config.batchProcessFreq
        << "\nMin instructions: " << config.minInstructions
        << "\nMax instructions: " << config.maxInstructions
        << "\nDelay per execution: " << config.delayPerExec
        << "\n=====================================================" << endl;
    return ss.str();
}


int main() {
    /*vector<Processs> processList;*/
   /* vector<CPU> cpuList;*/
   /* FCFS_Scheduler schedulerMain;*/
    ScreenManager screenManager;

    screenManager.handleCurrentCommand("scheduler -test");
       /* screenManager.push_back(Processs("process_" + std::to_string(i), i, 100));*/
        /*processList.push_back(Processs("process_" + std::to_string(i), i, 100));*/

   /* for (int i = 0; i < 10; ++i) {
        processList.push_back(Processs("process_" + std::to_string(i), i, 100));
    }*/
   /* for (int i = 0; i < 4; ++i) {
        cpuList.push_back(CPU(i));
    }*/
    /*screenManager.handleCurrentCommand("scheduler -test");*/

    /*for (Processs& process : screenManager.getScheduler().getProcessList()) {
        string name = process.getPName();
        int id = process.getPID();
        int num = process.getTotalInstructions();
        cout << name << " " << id << " " << num;
    }*/

    auto& schedulerMain = screenManager.getScheduler();
    auto& processList  = screenManager;
    auto& cpuList = schedulerMain.get_cpuList();

    for (Processs process : processList) {
        string name = process.getPName();
        int id = process.getPID();
        int num = process.getTotalInstructions();
        cout << name << " " << id << " " << num << "\n";
    }

    schedulerThread = thread([&schedulerMain, &processList, &cpuList]() {
        schedulerMain.runScheduler(processList, cpuList);
        
        });

    string command;
    while (true) {
        cout << "\nEnter Command: ";
        getline(cin, command);

        if (command == "screen -ls") {
            schedulerMain.displayProcesses();
        }
        else if (command == "exit") {
            schedulerMain.stopScheduler();
            break;
        }
        else {
            cout << "Unknown command: " << command << endl;
        }
    }

    if (schedulerThread.joinable()) {
        schedulerThread.join();
    }

    return 0;
}
