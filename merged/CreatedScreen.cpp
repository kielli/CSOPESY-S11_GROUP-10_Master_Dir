#include "CreatedScreen.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
//#include "../../source/repos/MCO1/MCO1/KeyboardEventHandler.h"
//#include "../../source/repos/MCO1/MCO1/MarqueeConsole.h"
#include <thread>

// Updated constructor to accept a ScreenManager pointer
CreatedScreen::CreatedScreen(const std::string& name, ScreenManager* manager)
    : screenName(name), creationTimestamp(getTimestamp()), screenManager(manager) {
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
    //if (command == "marquee") {
    //    MarqueeConsole marqueeConsole;
    //    KeyboardEventHandler keyboardEvent;
    //    atomic<bool> running(true);

    //    // Create threads for polling keyboard and updating marquee
    //    thread keyboardThread(PollKeyboard, ref(keyboardEvent), ref(marqueeConsole), ref(running));
    //    thread marqueeThread(MarqueeConsole::UpdateMarquee, ref(marqueeConsole), ref(running));

    //    keyboardThread.join(); // Wait for the keyboard thread to finish
    //    marqueeThread.join(); // Wait for the marquee thread to finish
    //}
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
