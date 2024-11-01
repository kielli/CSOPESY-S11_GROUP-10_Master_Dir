#include "MainScreen.h"
#include "ScreenManager.h"

#include <iostream>
#include <sstream>
#include<thread>
#include<chrono>
#include <atomic>

// Main screen constructor to initialize the screen manager
MainScreen::MainScreen(ScreenManager* manager) : screenManager(manager) {
    this->pName = "main";
    this->pId = this->screenManager->getTotalProcess();

    commands["screen"] = [this](const string& args)
        {
            stringstream iss(args);
            string option, screenName;

            iss >> option >> screenName;

            if (option == "-s" && !screenName.empty()) {
                // this->printAndStore("screen -s " + screenName + " recognized. Doing something.");
                screenManager->createScreen(screenName);
            }
            else if (option == "-r" && !screenName.empty()) {
                // this->printAndStore("screen -r " + screenName + " recognized, doing something.");
                screenManager->resumeScreen(screenName);
            }
            else if (option == "-ls") {
                 this->printAndStore("screen -ls command recognized, doing something.");
            }
            else {
                this->printAndStore("Invalid command format.Use: screen - s screenname or screen - r screenname\n");
            }
        };

    commands["scheduler"] = [this](const string& args)
        {
            stringstream iss(args);
            string option;
            iss >> option;

            if (option == "-test" ) {
                this->printAndStore("scheduler -test command recognized. Doing something.");
                this->runSchedulerTest();
               
            }
            else if (option == "-stop") {
                schedulerStop = true;
                this->printAndStore("scheduler -stop command recognized, doing something.");
            }
        };

    commands["report-util"] = [this](const string& args) {
        this->printAndStore("report-util command recognized. Doing something.\n");
        };

    commands["clear"] = [this](const string& args) {
        this->deleteContent(this->contents);
        this->display();
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

void MainScreen::handleCommand(const string& command) {
    istringstream iss(command); // parsing command string
    string cmd, args;

    iss >> cmd;
    getline(iss, args);

    // if command is found in the command map, pass the rest of the input as arguments
    if (commands.find(cmd) != commands.end()) {
        commands[cmd](args);
    }
    else {
        this->printAndStore("Unknown command: " + command);
    }
}

bool MainScreen::isExitRequested() const {
    return isExit;
}

int MainScreen::countScreens() const {
    return screenCount;
}

void MainScreen::runSchedulerTest() {
    int counter = screenManager->getTotalProcess();
    int cpuCycleCounter = 0;

    while (schedulerStop == false) {
        if (_kbhit()) {
            this->screenManager->PollKeyboard(this->screenManager->getKEH());
            this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        if (cpuCycleCounter % screenManager->getbatchProcessFreq() == 0) {
            string pname = "Process_" + std::to_string(counter);

            screenManager->createDummyScreen(pname);
            this->printAndStore("Dummy " + pname + " created");
            counter++;
        }

        this_thread::sleep_for(std::chrono::milliseconds(10));
        cpuCycleCounter++;
    }
    screenManager->setCpuCounterCycle(cpuCycleCounter);
}