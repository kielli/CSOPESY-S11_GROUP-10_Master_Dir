#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <string>
#include <vector>
#include <thread>

#include "Process.cpp"
#include "CPU_Core.cpp"
#include "FCFS_Scheduler.cpp"

using namespace std;

void menu(){
    cout << "\033[32;1mWelcome to the CSOPESY Terminal!\033[0m" << endl;
    cout << "\033[33mType 'exit' to quit, Type 'clear to clear screen\033[0m" << endl;
}

int main() {
    vector<Process> processList;
    vector<CPU_Core> cpuList;
    FCFS_Scheduler schedulerMain;

    for (int i = 0; i < 10; ++i) {
        processList.push_back(Process("process_" + std::to_string(i), i, 100));
    }

    for (int i = 0; i < 4; ++i) {
        cpuList.push_back(CPU_Core(i));
    }

    thread schedulerThread([&schedulerMain, &processList, &cpuList]() {
        schedulerMain.runScheduler(processList, cpuList);
    });

    string command;
    menu();
    while (true) {
        cout << "\nEnter Command: ";
        getline(cin, command);

        if (command == "screen -ls") {
            schedulerMain.displayProcesses();
        } else if (command == "exit") {
            schedulerMain.stopScheduler();
            break;
        } else {
            cout << "Unknown command: " << command << endl;
        }
    }

    if (schedulerThread.joinable()) {
        schedulerThread.join();
    }

    return 0;
}