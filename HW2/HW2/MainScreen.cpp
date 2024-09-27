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
            /*std::cout << "Creating a new screen with name: " << screenName << "\n";*/
            this->printAndStore("screen -s" + screenName + "recognized, doing something");
            screenManager->createScreen(screenName);
        }
        else if (option == "-r" && !screenName.empty()) {
            this->printAndStore("screen -r" + screenName + "recognized, doing something");
            /*std::cout << "Resuming screen: " << screenName << "\n";*/
            screenManager->resumeScreen(screenName);
        }
        else {
            /*std::cout << "Invalid command format. Use: screen -s screenname or screen -r screenname\n";*/
            this->printAndStore("Invalid command format.Use: screen - s screenname or screen - r screenname\n");
        }
        };

    commands["exit"] = [this](const std::string&) {
        std::cout << "Exiting program...\n";
        isExit = true;
        exit(0);
        };
}

void MainScreen::display() {
    system("cls");
    this->displayHeader();
    this->redrawScreen();
}

void MainScreen::displayHeader() {
    std::cout << "  ____   ____    ____   _____   _____   ____ __    __\n";
    std::cout << " / ___] / ___]  / __ \\ |  __ \\ | ____] / ___]\\ \\  / /\n";
    std::cout << "| |    | (___  | /  \\ || |__) || |___ | (___  \\ \\/ /\n";
    std::cout << "| |     \\___ \\ | |  | || ____/ | ____] \\___ \\  \\  /\n";
    std::cout << "| \\___  ____) || \\__/ || |     | |___  ____) | |  |\n";
    std::cout << " \\____][_____/  \\____/ |_|     |_____][_____/  |__|\n\n";
    std::cout << "Hello, Welcome to CSOPESY command line!\n";
    std::cout << "Type 'exit' to quit, 'clear' to clear the screen\n";
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
