#include "ScreenManager.cpp"
#include "Screen.cpp"
#include "CreatedScreen.cpp"
#include "MainScreen.cpp"
#include "Process.cpp"
#include "CPU.cpp"
#include "FCFS_Scheduler.cpp"
#include "RR_Scheduler.cpp"
#include "Scheduler.cpp"

// for visual studio uncomment
// #include "ScreenManager.h"

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
	thread schedulerThread;

    while (!screenManager.isMainScreenExitRequested())  // Check if exit is requested
    {
        screenManager.displayCurrentScreen();

        cout << "\033[37m" << "\nEnter a command: "; // white text
        getline(cin, command); // User input
        cout << "\n";
        cout.flush(); 

        if(command != "initialize" && screenManager.getInitializedState() == false)
        {
			if(command == "exit") {
                screenManager.handleCurrentCommand(command);
			}
			else{
                screenManager.invalidCommand(command);
                screenManager.addContent("'initialize' the program first.\n");
			}
        }
        else if(command == "initialize" && screenManager.getInitializedState() == false) {
            screenManager.initializeCommand();
        }
        else if(screenManager.getInitializedState() == true){
            screenManager.handleCurrentCommand(command);
        }

        // auto& scheduler = screenManager.getScheduler();
        // auto& processList = screenManager.getScheduler().getProcessList();
        // auto& cpuList = scheduler.get_cpuList();

       	// schedulerThread = thread ([&scheduler, &processList, &cpuList]() {
        //     scheduler.runScheduler(processList, cpuList);
        // });

        // if (schedulerThread.joinable()) {
        //     schedulerThread.join();
        // }
    }
    cout << "Exiting the program." << endl; 
    
    return 0;
}
