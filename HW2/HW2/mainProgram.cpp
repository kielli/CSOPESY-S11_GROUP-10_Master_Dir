#include "ScreenManager.h"
#include <iostream>

int main() {
    ScreenManager manager;

    manager.displayHeader();
    // Main loop to handle user input and display the current screen
    while (!manager.isMainScreenExitRequested()) {
        manager.displayCurrentScreen();  // Display the current screen
        // Simulating user command input
        std::string command;
        std::cout << "Enter command: ";
        std::getline(std::cin, command);

        manager.handleCurrentCommand(command);  // Handle user commands
    }

    std::cout << "Program exited.\n";
    return 0;
}
