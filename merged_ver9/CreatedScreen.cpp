#include "CreatedScreen.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

// Updated constructor to accept a ScreenManager pointer
CreatedScreen::CreatedScreen(const std::string& name, ScreenManager* manager, int numInstruction)
    : screenName(name), creationTimestamp(getTimestamp()), screenManager(manager) {
    this->pName = this->screenName;
    this->numInstructions = numInstruction;
}

void CreatedScreen::display() {
    system("cls");
    this->displayHeader();
    this->redrawScreen();
}

void CreatedScreen::displayHeader() {
    cout << "Process: " << this->getScreenName() << endl;
    cout << "Line: " << this->updateNumberOfLines() << " / " << this->getTotalLines() << std::endl;
    cout << "Created at: " << this->creationTimestamp << endl;
}

void CreatedScreen::displayProcessSmi() {
    cout << "Process: " << this->pName << endl;
    cout << "ID: " << this->pId << endl;
    cout << "\n";
    cout << "Current instruction line: " << this->currentLineNumber << endl;
    cout << "Lines of code: " << this->getTotalInstructions() << endl;
}

void CreatedScreen::handleCommand(const string& command) {
    if (command == "exit") {
        this->currentLineNumber = 1;

        screenManager->switchToMainScreen();  // Switch to main screen
        this->displayHeader();
        screenManager->displayCurrentScreen();
    }
    else {
        this->print_error(command);  // Handle invalid command
    }
}

const string& CreatedScreen::getScreenName() const {
    return screenName;
}