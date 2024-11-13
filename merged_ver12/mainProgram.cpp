//for vscode, uncomment
//#include "ScreenManager.cpp"
//#include "Screen.cpp"
//#include "CreatedScreen.cpp"
//#include "MainScreen.cpp"
//#include "KeyboardEventHandler.cpp"
//#include "Process.cpp"
//#include "Instruction.cpp"
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

using namespace std;

struct Config {
    int numCPU;
    string scheduler;
    int quantumCycles;
    int batchProcessFreq;
    int minInstructions;
    int maxInstructions;
    int delayPerExec;

    // functions to set config values, convert string to an int
    void setNumCPU(const string& value) {
        numCPU = stoi(value);
    }

    void setQuantumCycles(const string& value) {
        quantumCycles = stoi(value);
    }

    void setBatchProcessFreq(const string& value) {
        batchProcessFreq = stoi(value);
    }

    void setMinInstructions(const string& value) {
        minInstructions = stoi(value);
    }

    void setMaxInstructions(const string& value) {
        maxInstructions = stoi(value);
    }

    void setDelayPerExec(const string& value) {
        delayPerExec = stoi(value);
    }

    void setScheduler(const string& value) {
        scheduler = value;
    }
};

// Struct that holds the pointer to a string (key), and
// pointer to setter function (setter)
struct configEntries {
    const char* key;
    void (Config::* setter)(const string&);
};

// Maps each config keys to their corresponding setter functions
const vector<configEntries> configMap = {
    {"num-cpu", &Config::setNumCPU},
    {"scheduler", &Config::setScheduler},
    {"quantum-cycles", &Config::setQuantumCycles},
    {"batch-process-freq", &Config::setBatchProcessFreq},
    {"min-ins", &Config::setMinInstructions},
    {"max-ins", &Config::setMaxInstructions},
    {"delay-per-exec", &Config::setDelayPerExec}
};

Config readFile() {
    Config config;
    string line;
    ifstream inputFile("config.txt");

    if (!inputFile.is_open()) {
        cerr << "Unable to open file config.txt" << endl;
        return config; // return default-initialized config
    }

    // Loop to read the file's each line
    while (getline(inputFile, line)) {
        // Checks if it contains the the keys in the configMap
        for (const auto& configEntry : configMap) {
            // If the key is a match, it calls the setter function
            if (line.find(configEntry.key) != string::npos) {
                // Extracts the value after the " ", then calls the setter function
                (config.*configEntry.setter)(line.substr(line.find(" ") + 1));
                break; // Move to the next line after finding a match
            }
        }
    }

    inputFile.close();
    cout << "Config read successfully." << endl;

    return config;
}

string printConfig(const Config& config) {
    stringstream ss;
    ss << "\nLIST OF CONFIGURATION\n";
    ss << "Number of CPU: " << config.numCPU
        << "\nScheduler: " << config.scheduler
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
    Config config;

    // Main loop to handle user input and display the current screen
    while (!screenManager.isMainScreenExitRequested())
    {
        screenManager.displayCurrentScreen();

        cout << "\033[37m" << "\nEnter a command: "; // white text
        getline(std::cin, command); // User input
        cout << "\n";
        cout.flush();

        if (command != "initialize" && screenManager.getInitializeState() == false) {
            if (command == "exit") {
                screenManager.handleCurrentCommand(command);
            }
            else {
                screenManager.addContent("'initialize' program first.");
            }
        }
        else if (command == "initialize" && screenManager.getInitializeState() == false) {
            config = readFile(); // Read the config from the file and get its corresponding values
            screenManager.addContent("Program initialized...");
            screenManager.initialize(config.numCPU, config.scheduler, config.quantumCycles, config.batchProcessFreq, config.minInstructions, config.maxInstructions, config.delayPerExec);
            screenManager.addContent(printConfig(config));
        }
        else if (screenManager.getInitializeState() == true)
        {
            screenManager.addContent(command); // Adds content to the list of commands
            screenManager.handleCurrentCommand(command);  // Handles user commands

            if(config.scheduler == "\"fcfs\"")
            {
                auto& schedulerFCFS = screenManager.getScheduler();
                auto& processList = screenManager.getScheduler().getProcessList();
                auto & cpuList = schedulerFCFS.get_cpuList();

                thread schedulerThreadFCFS([&schedulerFCFS, &processList, &cpuList]() {
                    schedulerFCFS.runScheduler(processList, cpuList);
                });

                while (true) {
                    cout << "\nEnter Command: ";
                    getline(cin, command);

                    if (command.find("screen -s") == 0)
                    {
                        screenManager.handleCurrentCommand(command);
                        continue;
                    } 

                    if (command.find("screen -r") == 0)
                    {
                        screenManager.handleCurrentCommand(command);
                        continue;
                    } 

                    if (command == "scheduler -test") {
                        screenManager.handleCurrentCommand(command);
                        continue;
                    }
                    
                    if (command == "scheduler -stop") {
                        screenManager.handleCurrentCommand("scheduler -stop");
                        continue;
                    }

                    if (command == "screen -ls") {
                        schedulerFCFS.displayProcesses();
                        continue;
                    }
                    
                    if (command == "report -util")
                    {
                        ofstream reportfile;
                        streambuf* cout_buffer = cout.rdbuf(); // save the current buffer

                        if (!reportfile) {
                            cerr << "Unable to open file." << endl;
                            return 1;
                        }

                        reportfile.open("csopesy-log.txt"); // will create the file or overwrite
                        cout.rdbuf(reportfile.rdbuf()); // redirect cout to the output file 
                        schedulerFCFS.displayProcesses(); // function that displays the output to save
                        cout.rdbuf(cout_buffer); // restore the original buffer
                        reportfile.close();

                        cout << "Report generated at csopesy-log.txt.\n";
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

                if (schedulerThreadFCFS.joinable()) {
                    schedulerThreadFCFS.join();
                }
            }
            else if(config.scheduler == "\"rr\"")
            {
                auto& schedulerRR = screenManager.getRRscheduler();
                auto& processList = screenManager.getRRscheduler().getProcessList();
                auto& cpuList = schedulerRR.get_cpuList();

                thread schedulerThreadRR([&schedulerRR, &processList, &cpuList]() {
                    schedulerRR.runScheduler(processList, cpuList);
                });

                while (true)
                {
                    cout << "\nEnter Command: ";
                    getline(cin, command);

                    if (command.find("screen -s") == 0)
                    {
                        screenManager.handleCurrentCommand(command);
                        continue;
                    } 

                    if (command.find("screen -r") == 0)
                    {
                        screenManager.handleCurrentCommand(command);
                        continue;
                    } 

                    if (command == "scheduler -test") {
                        screenManager.handleCurrentCommand(command);
                        continue;
                    }
                    
                    if (command == "scheduler -stop") {
                        screenManager.handleCurrentCommand("scheduler -stop");
                        continue;
                    }

                    if (command == "screen -ls") {
                        schedulerRR.displayProcesses();
                        continue;
                    }
                    
                    if (command == "report -util")
                    {
                        ofstream reportfile;
                        streambuf* cout_buffer = cout.rdbuf(); // save the current buffer

                        if (!reportfile) {
                            cerr << "Unable to open file." << endl;
                            return 1;
                        }

                        reportfile.open("csopesy-log.txt"); // will create the file or overwrite
                        cout.rdbuf(reportfile.rdbuf()); // redirect cout to the output file 
                        schedulerRR.displayProcesses(); // function that displays the output to save
                        cout.rdbuf(cout_buffer); // restore the original buffer
                        reportfile.close();

                        cout << "Report generated at csopesy-log.txt.\n";
                        continue;
                    }
                    
                    if (command == "clear") {
                        screenManager.handleCurrentCommand("clear");
                        continue;
                    }

                    if(command == "exit") {
                        screenManager.handleCurrentCommand("exit");
                        continue;
                    }

                    else {
                        screenManager.handleCurrentCommand(command);
                        continue;
                    }
                }

                if (schedulerThreadRR.joinable()) {
                    schedulerThreadRR.join();
                }
            }
        }
    }
    cout << "Program exited.\n";

    return 0;
}