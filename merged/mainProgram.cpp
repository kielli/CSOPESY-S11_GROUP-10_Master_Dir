#include "ScreenManager.h"

//for vscode, uncomment
//#include "ScreenManager.cpp"
//#include "Screen.cpp"
//#include "CreatedScreen.cpp"
//#include "MainScreen.cpp"

#include <iostream>
#include <string>

using namespace std;

int main() {
    ScreenManager screenManager;
    string command;

    // Main loop to handle user input and display the current screen
    while (!screenManager.isMainScreenExitRequested())
    {
        // Display the current screen
        screenManager.displayCurrentScreen();
        
        // Simulating user command input
        cout << "\033[37m" << "\nEnter a command: "; // white text
        getline(std::cin, command); // User input
        cout << "\n";
        
        screenManager.addContent(command); // Adds content to the list of commands
        screenManager.handleCurrentCommand(command);  // Handles user commands
    }

    cout << "Program exited.\n";
    return 0;
}