//for vscode, uncomment
#include "ScreenManager.cpp"
#include "Screen.cpp"
#include "CreatedScreen.cpp"
#include "MainScreen.cpp"
#include "KeyboardEventHandler.cpp"
#include "Process.cpp"
#include "Instruction.cpp"
#include "CPU.cpp"
#include "FCFS_Scheduler.cpp"
#include "RR_Scheduler.cpp"
#include "Scheduler.cpp"
#include "SchedulerManager.cpp"

// for visual studio uncomment
// #include "ScreenManager.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>

using namespace std;

int main() {
    ScreenManager screenManager;
    SchedulerManager schedulerManager;
    thread schedulerThread;
    string command;

    // Main loop to handle user input and display the current screen
    while (!screenManager.isMainScreenExitRequested())
    {
        screenManager.displayCurrentScreen();

        cout << "\033[37m" << "\nEnter a command: "; // white text
        getline(std::cin, command); // User input
        cout << "\n";
        cout.flush();

        if (command != "initialize" && screenManager.getInitializeState() == false)
        {
            if (command == "exit")
                screenManager.handleCurrentCommand(command);
            else
                screenManager.addContent("'initialize' the program first.");
        }
        else if(command == "initialize" && screenManager.getInitializeState() == false) {
            screenManager.initializeConfigEntries();
        }
        else if(screenManager.getInitializeState() == true) {
            screenManager.addContent(command); // Adds content to the list of commands
            screenManager.handleCurrentCommand(command);  // Handles user commands

            if(screenManager.getScheduler() == "\"fcfs\"")
            {
                schedulerManager.setScheduler("fcfs");

                auto& processList = schedulerManager.getProcessList();
                auto& cpuList = schedulerManager.get_cpuList();

                // Start the scheduler thread, correctly capturing schedulerManager and passing the processList and cpuList
                schedulerThread = thread([&schedulerManager, &processList, &cpuList]() {
                    schedulerManager.getScheduler().runScheduler(processList, cpuList);
                });

                while(true)
                {
                    cout << "\033[37m" << "\nEnter a command: "; // white text
                    getline(std::cin, command); // User input
                    cout << "\n";
                        
                    if(command == "scheduler -test") {
                        screenManager.handleCurrentCommand(command);
                        continue;
                    }
                    
                    if (command == "clear") {
                        screenManager.handleCurrentCommand("clear");
                        continue;
                    }

                    if (command == "exit") {
                        screenManager.handleCurrentCommand("exit");
                        continue;
                    }

                    else {
                        screenManager.handleCurrentCommand(command);
                        continue;
                    }
                }

                if (schedulerThread.joinable()) {
                    schedulerThread.join();
                }
            }
            else if(screenManager.getScheduler() == "\"rr\"")
            {
                schedulerManager.setScheduler("rr");

                auto& processList = schedulerManager.getProcessList();
                auto& cpuList = schedulerManager.get_cpuList();

                // Start the scheduler thread, correctly capturing schedulerManager and passing the processList and cpuList
                schedulerThread = thread([&schedulerManager, &processList, &cpuList]() {
                    schedulerManager.getScheduler().runScheduler(processList, cpuList);
                });

                while(true)
                {
                    cout << "\033[37m" << "\nEnter a command: "; // white text
                    getline(std::cin, command); // User input
                    cout << "\n";
                        
                    if(command == "scheduler -test") {
                        screenManager.handleCurrentCommand("scheduler");
                        continue;
                    }

                    if(command == "screen -s") {
                        screenManager.handleCurrentCommand("screen");
                        continue;
                    }

                    if(command == "screen -r") {
                        screenManager.handleCurrentCommand("screen");
                        continue;
                    }

                    if(command == "scheduler -stop") {
                        screenManager.handleCurrentCommand("scheduler");
                        continue;
                    }

                    if(command == "screen -ls") {
                        schedulerManager.displayProcesses();
                        continue;
                    }

                    if(command == "report -util") {
                        ofstream reportfile;
                        streambuf* cout_buffer = cout.rdbuf(); // save the current buffer

                        if (!reportfile) {
                            cerr << "Unable to open file." << endl;
                        }

                        reportfile.open("csopesy-log.txt"); // will create the file or overwrite
                        cout.rdbuf(reportfile.rdbuf()); // redirect cout to the output file 
                        schedulerManager.displayProcesses(); // function that displays the output to save
                        cout.rdbuf(cout_buffer); // restore the original buffer
                        reportfile.close();

                        screenManager.addContent("Report generated at csopesy-log.txt.\n");
                    }

                    if (command == "clear") {
                        screenManager.handleCurrentCommand("clear");
                        continue;
                    }

                    if (command == "exit") {
                        screenManager.handleCurrentCommand("exit");
                        continue;
                    }

                    else {
                        screenManager.handleCurrentCommand(command);
                        continue;
                    }
                }

                if (schedulerThread.joinable()) {
                    schedulerThread.join();
                }
            }
        }
    }
    cout << "Program exited.\n";

    return 0;
}