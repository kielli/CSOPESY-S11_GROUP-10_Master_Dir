#include "ScreenManager.h"
#include <iostream>

// constructor for ScreenManager, always adds the main screen at index 0
ScreenManager::ScreenManager() {
    // add the main screen at index 0
    screens.push_back(make_unique<MainScreen>(this));
    currentScreenIndex = 0;  // Set the current screen to the main screen
}

// Create a new named screen
void ScreenManager::createScreen(const std::string& screenName) {
    system("cls");
    if (screenMap.find(screenName) == screenMap.end()) {
        screens.push_back(std::make_unique<CreatedScreen>(screenName, this));  // pass the ScreenManager pointer to CreatedScreen
        screenMap[screenName] = screens.size() - 1;  // map screen name to index
        currentScreenIndex = screens.size() - 1;  // switch to the new screen
    }
    else {
        cout << "Screen with name '" << screenName << "' already exists!\n";
    }
}

// Resume a created screen by name
void ScreenManager::resumeScreen(const string& screenName) {
    if (screenMap.find(screenName) != screenMap.end()) {
        currentScreenIndex = screenMap[screenName];
        screens[currentScreenIndex]->redrawScreen();  // Redraw the screen when resuming
    }
    else {
        cout << "No screen with the name '" << screenName << "' found.\n";
    }
}

// Switch back to the main screen
void ScreenManager::switchToMainScreen() {
    currentScreenIndex = 0;  // Main screen is always at index 0
    system("cls");
    screens[currentScreenIndex]->redrawScreen();
}

// display the current screen
void ScreenManager::displayCurrentScreen() {
    screens[currentScreenIndex]->display();
}

// check if the main screen has requested an exit
bool ScreenManager::isMainScreenExitRequested() {
    return dynamic_cast<MainScreen*>(screens[0].get())->isExitRequested();
}

void ScreenManager::addContent(const string& content) { 
    screens[currentScreenIndex]->Store(content);
}

void ScreenManager::handleCurrentCommand(const string& command) {
    if (command == "marquee") {
        this->CreateMarqueeConsole();
    }
    else
    {
        screens[currentScreenIndex]->handleCommand(command);

        // if exiting from a created screen, switch to the main screen
        if (dynamic_cast<CreatedScreen*>(screens[currentScreenIndex].get()) && command == "exit") {
            switchToMainScreen();
        }
    }
}

void ScreenManager::CreateMarqueeConsole() {
    system("cls");
    screens[currentScreenIndex]->CreateMarquee();
}