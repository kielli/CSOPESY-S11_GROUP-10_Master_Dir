#include "MainScreen.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <thread>

// Main screen constructor to initialize the screen manager
MainScreen::MainScreen(ScreenManager* manager) : screenManager(manager) {
    commandMap["initialize"] = [this](const string& args) {
        string input;

        this->printAndStore("Initializing the program...");

        // Read config.txt file
        this->readFile();

        // Create processes and cores
        this->createProcesses();
        this->createCores();

        // Run the scheduler
        screenManager->fcfsRunScheduler(schedulerFCFS, processList, cpuList);

        commandMap["screen"] = [this](const string& args) {
            stringstream iss(args);
            string option, screenName;

            iss >> option >> screenName;

            if (option == "-s" && !screenName.empty()) { // for creating a new screen
                screenManager->createScreen(screenName);
            }
            else if (option == "-r" && !screenName.empty()) { // for resuming in created screen
                screenManager->resumeScreen(screenName);
            }
            else if(option == "-ls") {     
                this->printAndStore("CPU Utilization: \n");
                this->printAndStore("Cores used: \n");
                this->printAndStore("Cores available: \n");
            }
            else {
                this->printAndStore("Invalid command format. Use: screen -s <screenname> or screen -r <screenname>\n");
            }
        };  

        commandMap["scheduler-test"] = [this](const string& args) {
            this->printAndStore("scheduler-test command recognized. Doing something.\n");
        };

        commandMap["scheduler-stop"] = [this](const string& args) {
            this->printAndStore("scheduler-stop command recognized. Doing something.\n");
        };

        commandMap["report-util"] = [this](const string& args) {
            this->printAndStore("report-util command recognized. Doing something.\n");
        };

        // Clears the screen
        commandMap["clear"] = [this](const string& args) {
            this->deleteContent(this->contents);
            this->displayScreen();
        };
    };

    // Exits the program
    commandMap["exit"] = [this](const string&) {
        cout << "Exiting the program...\n";
        isExit = true;
        exit(0);
    };
}

// Displays the main screen header
void MainScreen::displayHeader() {
    cout << "  ____   ____    ____   _____   _____   ____ __    __\n";
    cout << " / ___] / ___]  / __ \\ |  __ \\ | ____] / ___]\\ \\  / /\n";
    cout << "| |    | (___  | /  \\ || |__) || |___ | (___  \\ \\/ /\n";
    cout << "| |     \\___ \\ | |  | || ____/ | ____] \\___ \\  \\  /\n";
    cout << "| \\___  ____) || \\__/ || |     | |___  ____) | |  |\n";
    cout << " \\____][_____/  \\____/ |_|     |_____][_____/  |__|\n\n";
    cout << "------------------------------------------------------------\n";
    cout << "\033[32m" << "Welcome to CSOPESY Emulator!\n\n"; // Green text

    cout << "\033[37m" << "Developers:\n";
    cout << "\t   Cipriaso, James Kielson\n";
    cout << "\t   Hallar, Francine Marie\n";
    cout << "\t   Hong, Letty\n";
    cout << "\t   Pe, Gyan Josh\n";

    cout << "\nLast updated:" + this->getTimestamp() + "\n";
    cout << "\n------------------------------------------------------------\n";
    cout << "\033[33m" << "Type 'exit' to quit, 'clear' to clear the screen, 'initialize' to start program\n"; // Yellow text
    cout << "\033[37m" << "\n"; // Reset to white text
}

// Displays the header and the redrawn screen
void MainScreen::displayScreen() {
    string command;

    system("cls");
    this->displayHeader();
    this->redrawScreen();
}

void MainScreen::handleCommand(const string& command) {
    istringstream iss(command); // parsing command string
    string cmd, args;

    iss >> cmd;
    getline(iss, args);

    // if command is found in the command map, pass the rest of the input as arguments
    if (commandMap.find(cmd) != commandMap.end()) {
        commandMap[cmd](args); 
    }
    else {
        this->print_error(command);
    }
}

void MainScreen::readFile() {
    ifstream inputFile("config.txt");
    string line;
    bool hasContent = false;

    this->printAndStore("Program initialized.\n");

    if (!inputFile) {
        this->printAndStore("File 'config.txt' does not exist.\n");
        return; 
    } 

    while (getline(inputFile, line)) {
        // this->printAndStore(line);  // for checking if it read the contents
        hasContent = true;
    }

    if (!hasContent) {
        this->printAndStore("The file 'config.txt' is empty.\n");
    }

    inputFile.close();
}

void MainScreen::createProcesses() {
    for (int i = 0; i < 10; ++i) {
        processList.push_back(Process("process_" + std::to_string(i), i, 100));
    }
}

void MainScreen:: createCores() {
    for (int i = 0; i < 4; ++i) {
        cpuList.push_back(CPU_Core(i));
    }
}

bool MainScreen::isExitRequested() const {
    return isExit;
}