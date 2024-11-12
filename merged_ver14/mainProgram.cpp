//#include "ScreenManager.cpp"
//#include "Screen.cpp"
//#include "CreatedScreen.cpp"
//#include "MainScreen.cpp"
//#include "Process.cpp"
//#include "CPU.cpp"
//#include "FCFS_Scheduler.cpp"
//#include "RR_Scheduler.cpp"

// for visual studio uncomment
#include "ScreenManager.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <thread>

using namespace std;

int main()
{
	ScreenManager screenManager;
    string command;
	thread schedulerThreadFCFS,  schedulerThreadRR;

    while (screenManager.getInitializedState() == false) {
        screenManager.displayCurrentScreen();
        screenManager.initializeCommand();
    }

    while (!screenManager.isMainScreenExitRequested()) 
    {
        screenManager.displayCurrentScreen();

        if(screenManager.getSchedulerType() == "\"fcfs\"")
        {
            auto& schedulerFCFS = screenManager.getFCFSScheduler();
            auto& processList = screenManager.getFCFSScheduler().getProcessList();
            auto & cpuList = schedulerFCFS.get_cpuList();

            schedulerThreadFCFS = thread ([&schedulerFCFS, &processList, &cpuList]() {
                schedulerFCFS.runScheduler(processList, cpuList);
            });

        }
        else if(screenManager.getSchedulerType() == "\"rr\"")
        {
            auto& schedulerRR = screenManager.getRRscheduler();
            auto& processList = screenManager.getRRscheduler().getProcessList();
            auto& cpuList = schedulerRR.get_cpuList();

            schedulerThreadRR = thread ([&schedulerRR, &processList, &cpuList]() {
                schedulerRR.runScheduler(processList, cpuList);
            });
        }

        cout << "\033[37m" << "\nEnter a command: "; // white text
        getline(cin, command); // User input
        cout << "\n";
        cout.flush(); 

        screenManager.handleCurrentCommand(command);

        if(screenManager.getSchedulerType() == "\"fcfs\"") {
           if (schedulerThreadFCFS.joinable()) {
                schedulerThreadFCFS.join();
            }
        }
        else if(screenManager.getSchedulerType() == "\"rr\"") {
            if (schedulerThreadRR.joinable()) {
                schedulerThreadRR.join();
            }
        }
    }

    cout << "Exiting the program." << endl; 
    
    return 0;
}
