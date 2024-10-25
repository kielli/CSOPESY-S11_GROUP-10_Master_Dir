#include "MainScreen.h"
#include "ScreenManager.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <thread>

//#include "../../source/repos/MCO1/MCO1/KeyboardEventHandler.h"
//#include "../../source/repos/MCO1/MCO1/MarqueeConsole.h"

// Main screen constructor
MainScreen::MainScreen(ScreenManager* manager) : screenManager(manager) {
    commands["initialize"] = [this](const string& args) {
        this->printAndStore("Initializing the program...\n");

        // Read config.txt file
        this->readFile();
        
        // Create processes and CPU cores
        this->createProcessesAndCores();

        commands["screen"] = [this](const string& args) {
            stringstream iss(args);
            string option, screenName;

            iss >> option >> screenName;

            if (option == "-s" && !screenName.empty()) {
                screenManager->createScreen(screenName);
            }
            else if (option == "-r" && !screenName.empty()) {
                screenManager->resumeScreen(screenName);
            }
            else {
                this->printAndStore("Invalid command format. Use: screen -s <screenname> or screen -r <screenname>\n");
            }

            if (option == "-ls") {            
                // Run scheduler
                this->startSchedulerFCFS(FCFSScheduler, processList, cpuList);

                this_thread::sleep_for(std::chrono::seconds(4));

                // Display processes
                screenManager->getScheduler();

                // this->stopSchedulerFCFS();
            }
        };

        commands["scheduler-test"] = [this](const string& args) {
            this->printAndStore("scheduler-test command recognized. Doing something.\n");
        };

        commands["scheduler-stop"] = [this](const string& args) {
            this->printAndStore("scheduler-stop command recognized. Doing something.\n");
        };

        commands["report-util"] = [this](const string& args) {
            this->printAndStore("report-util command recognized. Doing something.\n");
        };

        commands["clear"] = [this](const string& args) {
            this->deleteContent(this->contents);
            this->display();
        };
    };

    commands["exit"] = [this](const string&) {
        cout << "Exiting program...\n";
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
void MainScreen::display() {
    system("cls");
    this->displayHeader();
    this->redrawScreen();
}

string truncateText(const string& text, size_t maxLength) {
    if (text.length() > maxLength) {
        return text.substr(0, maxLength - 3) + "...";  // Truncate and add "..."
    }
    return text;
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

void MainScreen::handleCommand(const string& command) {
    istringstream iss(command);
    string cmd, args;

    iss >> cmd;
    getline(iss, args);

    if (commands.find(cmd) != commands.end()) {
        commands[cmd](args);  // pass the rest of the input as arguments
    }
    else {
        this->printAndStore("Unknown command: " + command + "\n");
    }
}

bool MainScreen::isExitRequested() const {
    return isExit;
}

int MainScreen::countScreens() const {
    return screenCount;
}

string MainScreen::getTimestamp() const {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm localTime; //tm structure to hold the local time

    // use localtime_s for thread safety
    localtime_s(&localTime, &currentTime);

    stringstream ss;
    ss << put_time(&localTime, "%a %b %d %H:%M:%S %Y");
    return ss.str();
}