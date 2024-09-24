#include <iostream>
#include <string>
#include <cstdlib>
#include "ScreenManager.h"
#include "Screen.h"

using namespace std;

int start_program();
void print_header();
void process_command(const std::string& command, ScreenManager& screenManager, Screen*& currentScreen);
void print_process(const std::string& command);
void print_error(const std::string& command);

int main() {
    start_program();
    return 0;
}

int start_program() {
    ScreenManager screenManager;
    vector<Screen*> screens;
    print_header();
    Screen* currentScreen = nullptr;

    while (true) {
        if (currentScreen != nullptr && currentScreen->isRunning()) {
            // Display current screen info if in screen session
            currentScreen->displayScreen();
            string input;
            cout << "Enter command (or type anything to add content): ";
            std::getline(cin, input);

            // Check if the user wants to exit
            if (input == "exit") {
                currentScreen->stop();
                //currentScreen = nullptr;
            }
            else {
                // Automatically add any non-exit input as content
                currentScreen->addContent(input);
            }
        }
        else {
            std::cout << "Main Menu: Type 'screen -r <name>' to resume, 'screen -s <name>' to start a new screen, or 'exit' to quit." << endl;
            std::cout << "Enter command: ";
            string command;
            std::getline(cin, command);
            process_command(command, screenManager, currentScreen);
        }
    }

    return 0;
}

void print_header() {
    std::cout << "  ____   ____    ____   _____   _____   ____ __    __\n";
    std::cout << " / ___] / ___]  / __ \\ |  __ \\ | ____] / ___]\\ \\  / /\n";
    std::cout << "| |    | (___  | /  \\ || |__) || |___ | (___  \\ \\/ /\n";
    std::cout << "| |     \\___ \\ | |  | || ____/ | ____] \\___ \\  \\  /\n";
    std::cout << "| \\___  ____) || \\__/ || |     | |___  ____) | |  |\n";
    std::cout << " \\____][_____/  \\____/ |_|     |_____][_____/  |__|\n\n";
    std::cout << "Hello, Welcome to CSOPESY command line!\n";
    std::cout << "Type 'exit' to quit, 'clear' to clear the screen\n";
}

void process_command(const std::string& command, ScreenManager& screenManager, Screen*& currentScreen) {
    string screenName;

    if (command == "initialize") {
        print_process(command);
    }
    else if (command == "screen") {
        print_process(command);
    }
    else if (command == "scheduler-test") {
        print_process(command);
    }
    else if (command == "scheduler-stop") {
        print_process(command);
    }
    else if (command == "report-util") {
        print_process(command);
    }
    else if (command == "clear") {
        system("cls");
        print_header();
    }
    if (command.substr(0, 9) == "screen -s") {
        screenName = command.substr(10);
        screenManager.createScreen(screenName);
        currentScreen = screenManager.getScreen(screenName); // Get the newly created screen
    }
    else if (command.substr(0, 9) == "screen -r") {
        screenName = command.substr(10);
        screenManager.resumeScreen(screenName);
        currentScreen = screenManager.getScreen(screenName); // Get the resumed screen
        currentScreen->displayScreen();
    }
    /*else if (command == "list") {
        screenManager.listScreens();
    }*/
    else if (command == "exit") {
        if (currentScreen != nullptr && currentScreen->isRunning()) {
            currentScreen->stop(); // Stop the current screen and return to main menu
            currentScreen = nullptr; 
        }
        else {
            cout << "Exiting the program..." << endl;
            exit(0);
        }
    }
    else {
        print_error(command);
    }
}

void print_process(const std::string& command) {
    std::cout << command << " command recognized. Doing something.\n";
}

void print_error(const std::string& command) {
    std::cout << "Unknown command: " << command << ".\n";
}
