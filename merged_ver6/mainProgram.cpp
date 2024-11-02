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
   /* bool isvalid1, isvalid2, isvalid3, isvalid4, isvalid5 = false;
    bool isvalid = false;*/

   /* while (isvalid == false) {
        while (isvalid1 == false) {
            cout << "number of CPU: ";
            cin >> config.numCPU;
            cin.ignore();
            if (config.numCPU >= 1 && config.numCPU <= 128) {
                isvalid1 = true;
                break;
            }
        }

        while (isvalid2 == false) {
            cout << "Scheduler type: ";
            getline(cin, config.scheduler);
            if (config.scheduler =="fcfs" || config.scheduler == "rr") {
                isvalid2 = true;
                break;
            }
        }
        while (isvalid3 == false) {
            cout << "Quantum cycles: ";
            cin >> config.quantumCycles;
            if (config.quantumCycles >= 1 && config.quantumCycles <= pow(2,32)) {
                isvalid3 = true;
                break;
            }
        }
        while (isvalid4 == false) {
            cout << "Batch process frequency: ";
            cin >> config.batchProcessFreq;
            if (config.batchProcessFreq >= 1 && config.batchProcessFreq <= pow(2, 32)) {
                isvalid3 = true;
                break;
            }
        }
    }*/
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
    ScreenManager screenManager;
    string command;

    // Main loop to handle user input and display the current screen
    while (!screenManager.isMainScreenExitRequested())
    {
        screenManager.displayCurrentScreen();
        cout << "\033[37m" << "\nEnter a command: "; // white text
        getline(std::cin, command); // User input
        /*cout << "\n";*/

        if (command!="initialize" && screenManager.getInitializeState() == false) {
            if (command == "exit") {
                screenManager.handleCurrentCommand(command);
            }
            else {
                screenManager.addContent("initialize first");
            }
        }

        else if (command == "initialize" && screenManager.getInitializeState()== false){
            Config config = initialize();
            screenManager.initialize(config.numCPU, config.scheduler, config.quantumCycles, config.batchProcessFreq, config.minInstructions, config.maxInstructions, config.delayPerExec);
            screenManager.addContent(printConfig(config));

        }

        else if (screenManager.getInitializeState() == true) {
            screenManager.addContent(command); // Adds content to the list of commands
            screenManager.handleCurrentCommand(command);  // Handles user commands
        }
       
    }

    cout << "Program exited.\n";
    return 0;
}