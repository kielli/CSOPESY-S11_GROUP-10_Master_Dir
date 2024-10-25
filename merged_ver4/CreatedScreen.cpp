#include "CreatedScreen.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>

// Updated constructor to accept a ScreenManager pointer
CreatedScreen::CreatedScreen(const std::string& name, ScreenManager* manager)
    : screenName(name), creationTimestamp(getTimestamp()), screenManager(manager) {
}

// Displays current screen
void CreatedScreen::displayScreen() { 
    system("cls");
    this->displayCreatedScreenHeader();
    this->redrawScreen();
}

// Displays the created screen's screen name, number of lines, and created time and date
void CreatedScreen::displayCreatedScreenHeader() {
    cout << "Process: " << this->getScreenName() << endl;
    cout << "Line: " << this->updateNumberOfLines() << " / " << this->getTotalLines() << std::endl;
    cout << "Created at: " << this->creationTimestamp << endl;
}

// Handles created screen commands
void CreatedScreen::handleCommand(const string& command) {
    if (command == "exit") {
        this->currentLineNumber = 1;

        screenManager->switchToMainScreen();  // Switch to main screen
        screenManager->displayCurrentScreen();
    }
    else {
        this->print_error(command);  // Handles invalid command
    }
}

// Get screen name 
const string& CreatedScreen::getScreenName() const {
    return screenName;
}