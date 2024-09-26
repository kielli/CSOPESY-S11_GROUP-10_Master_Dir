#include "MainScreen.h"
#include "ScreenManager.h"
#include <iostream>
#include <sstream>

MainScreen::MainScreen(ScreenManager* manager) : screenManager(manager) {
    commands["screen"] = [this](const std::string& args) {
        std::istringstream iss(args);
        std::string option, screenName;
        iss >> option >> screenName;

        if (option == "-s" && !screenName.empty()) {
            std::cout << "Creating a new screen with name: " << screenName << "\n";
            screenManager->createScreen(screenName);
        }
        else if (option == "-r" && !screenName.empty()) {
            std::cout << "Resuming screen: " << screenName << "\n";
            screenManager->resumeScreen(screenName);
        }
        else {
            std::cout << "Invalid command format. Use: screen -s screenname or screen -r screenname\n";
        }
        };

    commands["exit"] = [this](const std::string&) {
        std::cout << "Exiting program...\n";
        isExit = true;
        exit(0);
        };
}

void MainScreen::display() {
}

void MainScreen::handleCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd, args;
    iss >> cmd;
    std::getline(iss, args);

    if (commands.find(cmd) != commands.end()) {
        commands[cmd](args);  // pass the rest of the input as arguments
        this->printAndStore(command + " command recognized. Doing something.");

    }
    else {
        this->printAndStore("Unknown command : " + command);
    }
}

bool MainScreen::isExitRequested() const {
    return isExit;
}

int MainScreen::countScreens() const {
    return screenCount;
}
