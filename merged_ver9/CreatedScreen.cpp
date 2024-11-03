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
    cout << "\n";
}

void CreatedScreen::displayProcessSmi() {
    this->printAndStore("\nProcess: " + this->pName + "\n");
    this->printAndStore("ID: " + this->pId);
    // this->printAndStore("\n\n");
    // this->printAndStore("Current instruction line: ");
    // this->printAndStore("\n");
    // this->printAndStore("Lines of code: " + this->getTotalInstructions());
    // this->printAndStore("\n");
}

void CreatedScreen::handleCommand(const string& command) {
    if (command == "process -smi") {
        this->displayProcessSmi();
    }
    else if (command == "exit") {
        this->currentLineNumber = 1;

        screenManager->switchToMainScreen();  // Switch to main screen
        screenManager->displayCurrentScreen();
    }
    else {
        this->print_error(command);  // Handle invalid command
    }
}

const string& CreatedScreen::getScreenName() const {
    return screenName;
}