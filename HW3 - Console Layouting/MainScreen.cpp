#include "MainScreen.h"
#include "ScreenManager.h"
#include <iostream>
#include <sstream>

// Main screen constructor
MainScreen::MainScreen(ScreenManager* manager) : screenManager(manager) {
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
        else {
            this->printAndStore("Invalid command format.Use: screen - s screenname or screen - r screenname\n");
        }
    };

    commands["initialize"] = [this](const string& args) {
        this->printAndStore("initialize command recognized. Doing something.\n");
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
    cout << "\033[32m" << "Hello, welcome to CSOPESY commandline!\n"; // green text
    cout <<  "\033[33m" << "Type 'exit' to quit, 'clear' to clear the screen\n"; // yellow text
    cout << "\033[37m" << "\n"; // white text"
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
        this->printAndStore("Unknown command: " + command);
    }
}

bool MainScreen::isExitRequested() const {
    return isExit;
}

int MainScreen::countScreens() const {
    return screenCount;
}