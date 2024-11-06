#include "CreatedScreen.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

void CreatedScreen::display() {
    system("cls");
    this->displayHeader();
    this->redrawScreen();
}

void CreatedScreen::displayHeader() {
    cout << "Process: " << this->getPName() << endl;
    cout << "Line: " << this->updateNumberOfLines() << " / " << this->getTotalLines() << std::endl;
    cout << "Created at: " << this->getTimestamp() << endl;
    cout << "Curren screen index: " << to_string(screenManager->getCurrentScreenIndex()) << endl;
    cout << "\n";
}

void CreatedScreen::displayProcessSmi() {
    int remainingIns = this->getRemainingI();
    int totalIns = this->getTotalInstructions();
    int pId = this->getPID();

    if(remainingIns != totalIns) {
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
    if(command == "process -smi") {
        this->displayProcessSmi();
    } 
    else if (command == "exit") {
        this->currentLineNumber = 1;

        screenManager->switchToMainScreen();  // Switch to main screen
        screenManager->displayCurrentScreen();
    } else {
        this->print_error(command);  // Handle invalid command
    }
}

const string& CreatedScreen::getScreenName() const {
    return screenName;
}