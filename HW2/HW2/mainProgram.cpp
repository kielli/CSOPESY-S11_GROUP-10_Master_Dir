#include "ScreenManager.h"
#include <iostream>

int main() {
    ScreenManager manager;

    /*manager.displayHeader();*/
    // main loop to handle user input and display the current screen
    while (!manager.isMainScreenExitRequested()) {
        manager.displayCurrentScreen();  // Display the current screen
        // simulating user command input
        std::string command;
        std::cout << "Enter command: ";
        std::getline(std::cin, command);
        manager.addContent(command);
        manager.handleCurrentCommand(command);  // Handle user commands
    }

    std::cout << "Program exited.\n";
    return 0;
}
