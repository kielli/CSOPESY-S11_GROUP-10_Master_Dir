#include "CreatedScreen.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

// Updated constructor to accept a ScreenManager pointer
CreatedScreen::CreatedScreen(const std::string& name, ScreenManager* manager)
    : screenName(name), creationTimestamp(getTimestamp()), screenManager(manager) {
}

void CreatedScreen::display() { 
    /*std::cout << "  Process: " << this->getScreenName() << std::endl;
    std::cout << "  line: " << this->updateNumberOfLines() << " / " << this->getTotalLines() << std::endl;
    std::cout << "  created at: " << this->creationTimestamp << std::endl;*/
    system("cls");
    this->displayHeader();
    this->redrawScreen();
}

void CreatedScreen::displayHeader() {
    std::cout << "  Process: " << this->getScreenName() << std::endl;
    std::cout << "  line: " << this->updateNumberOfLines() << " / " << this->getTotalLines() << std::endl;
    std::cout << "  created at: " << this->creationTimestamp << std::endl;
}


void CreatedScreen::handleCommand(const std::string& command) {
    if (command == "exit") {
        this->currentLineNumber = 1;
        this->deleteContent(this->contents);
        screenManager->switchToMainScreen();  // switch to main screen
        this->displayHeader();
        screenManager->displayCurrentScreen();
    }
    else {
        this->print_error(command);  // handle invalid command
    }
}

const std::string& CreatedScreen::getScreenName() const {
    return screenName;
}

std::string CreatedScreen::getTimestamp() const {
    auto now = std::chrono::system_clock::now();
    time_t currentTime = std::chrono::system_clock::to_time_t(now);
    tm localTime; //tm structure to hold the local time

    // use localtime_s for thread safety
    localtime_s(&localTime, &currentTime);

    std::stringstream ss;
    ss << std::put_time(&localTime, "%m/%d/%Y, %I:%M:%S %p");
    return ss.str();
}
