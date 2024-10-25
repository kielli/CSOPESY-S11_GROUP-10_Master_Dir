#include "ScreenManager.cpp"
#include "Screen.cpp"
#include "MainScreen.cpp"
#include "CreatedScreen.cpp"

#include <iostream>
#include <string>

using namespace std;

int main() {
        string command;
        ScreenManager screenManager;

        // Main loop to handle user input and display the current screen
        while (!screenManager.isMainScreenExitRequested())
        {
            screenManager.displayCurrentScreen(); // Display the current screen
            screenManager.commandInput(command);
            screenManager.addContent(command); // Adds content to the list of commands
            screenManager.handleCurrentCommand(command);  // Handles user commands
        }
        
        cout << "Program exited.\n";

        return 0;
}