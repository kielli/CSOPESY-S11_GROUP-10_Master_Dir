#include "MainScreen.h"
#include "ScreenManager.h"
#include "SchedulerManager.h"

#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <atomic>
#include <random>
#include <cstdlib>   // for rand() and srand()
#include <ctime> 
#include <fstream>

// Main screen constructor
MainScreen::MainScreen(ScreenManager* manager) : screenManager(manager)
{
    this->pName = "main";
    this->pId = this->screenManager->getTotalProcess();

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
            this->print_error(option);
        }
    };

    commands["scheduler"] = [this](const string& args) {
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
    istringstream iss(command);
    string cmd, args;

    iss >> cmd;
    getline(iss, args);

    if (commands.find(cmd) != commands.end()) {
        commands[cmd](args);  // pass the rest of the input as arguments
    }
    else {
        this->print_error(command);
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

    while(schedulerStop == false) {
        if (_kbhit()) {
            this->screenManager->PollKeyboard(this->screenManager->getKEH());
            this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        if (cpuCycleCounter % 4/*screenManager->getbatchProcessFreq()*/ == 0) {
            string pname = "process" + to_string(counter);
            srand(static_cast<unsigned>(time(0) + 39 % 4));
            int instructionCount = screenManager->getMinInstructions() + rand() % (/*screenManager->getMaxInstructions() - screenManager->getMinInstructions()*/ 100 - 200 + 1);

            screenManager->createDummyScreen(pname, instructionCount);
            counter++;
        }
        this_thread::sleep_for(std::chrono::milliseconds(50));
        cpuCycleCounter++;
    }    
}