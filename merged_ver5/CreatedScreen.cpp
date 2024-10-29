#include "CreatedScreen.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

// Updated constructor to accept a ScreenManager pointer
CreatedScreen::CreatedScreen(const std::string& name, ScreenManager* manager)
    : screenName(name), creationTimestamp(this->getTimestamp()), screenManager(manager) {
    this->pName = this->screenName;
    this->pId = this->screenManager->getTotalProcess();
}

// Displays current screen
void CreatedScreen::display() {
    system("cls");
    this->displayHeader();
    this->redrawScreen();
}

// Displays the created screen's screen name, number of lines, and created time and date
void CreatedScreen::displayHeader() {
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
        this->print_error(command);  // Handle invalid command
    }
}

// Get screen name 
const string& CreatedScreen::getScreenName() const {
    return screenName;
}