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
    cout << "Process: " << this->getPName() << endl;
    cout << "Line: " << this->updateNumberOfLines() << " / " << this->getTotalLines() << std::endl;
    cout << "Created at: " << this->creationTimestamp << endl;
    cout << "/n";
}

void CreatedScreen::displayProcessSmi() {
    int remainingIns = this->getRemainingI();
    int totalIns = this->getTotalInstructions();
    int pId = this->getPID();
    if (remainingIns != totalIns) {
        this->printAndStore("\nProcess: " + this->pName);
        this->printAndStore("ID: " + to_string(pId) + "\n");
        this->printAndStore("Current instruction line: " + to_string(remainingIns));
        this->printAndStore("Lines of code: " + to_string(totalIns) + "\n");
    }
    else if (remainingIns == totalIns) {
        this->printAndStore("\nProcess: " + this->pName);
        this->printAndStore("ID: " + to_string(pId) + "\n");
        this->printAndStore("Finished!\n");
    }
}

void CreatedScreen::handleCommand(const string& command) {
    if (command == "exit") {
        this->currentLineNumber = 1;

        screenManager->switchToMainScreen();  // Switch to main screen
        screenManager->displayCurrentScreen();
    }
    else if (command == "process -smi") {
        this->displayProcessSmi();
    }
    else {
        this->print_error(command);  // Handle invalid command
    }
}

const string& CreatedScreen::getScreenName() const {
    return screenName;
}