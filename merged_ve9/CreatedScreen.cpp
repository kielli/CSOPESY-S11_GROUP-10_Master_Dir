#include "CreatedScreen.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

// Updated constructor to accept a ScreenManager pointer
CreatedScreen::CreatedScreen(const std::string& name, ScreenManager* manager, int numInstruction)
    : screenName(name), creationTimestamp(getTimestamp()), screenManager(manager){
    this->pName = this->screenName;
   /* this->pId = this->screenManager->getTotalProcess();*/
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

string CreatedScreen::getTimestamp() const {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm localTime; //tm structure to hold the local time

    // use localtime_s for thread safety
    localtime_s(&localTime, &currentTime);

    stringstream ss;
    ss << put_time(&localTime, "%m/%d/%Y, %I:%M:%S %p");
    return ss.str();
}