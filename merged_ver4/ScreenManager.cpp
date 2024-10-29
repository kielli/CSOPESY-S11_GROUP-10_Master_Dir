#include <iostream>
#include <thread>

#include "ScreenManager.h"
#include "MainScreen.h"
#include "CreatedScreen.h"

// Constructor for ScreenManager, always adds the main screen at index 0
ScreenManager::ScreenManager() {
    vectorScreen.push_back(make_unique<MainScreen>(this));   // Adds the main screen to the screen vector at index 0
    currentScreenIndex = 0;  // Sets the current screen to the main screen
}

// Simulating user command input
string ScreenManager::commandInput(string& command) {
    cout << "\033[37m" << "Enter a command: "; // white text
    getline(cin, command); // User input
    cout << "\n";
    cout.flush();

    return command;
}

string ScreenManager::commandScheduler(string& command) {
    cout << "Enter a scheduler command ['exit' to leave]: ";
    getline(cin, command); // User input
    cout.flush();

    return command;
}

// Displays the current screen
void ScreenManager::displayCurrentScreen() {
    vectorScreen[currentScreenIndex]->displayScreen();
}

// Create a new named screen
void ScreenManager::createScreen(const string& screenName) {
    system("cls");

    if (screenMap.find(screenName) == screenMap.end()) {
        vectorScreen.push_back(std::make_unique<CreatedScreen>(screenName, this));  // pass the ScreenManager pointer to CreatedScreen
        screenMap[screenName] = vectorScreen.size() - 1;  // Map screen name to index
        currentScreenIndex = vectorScreen.size() - 1;  // Switch to the new screen
    }
    else {
        cout << "Screen with name '" << screenName << "' already exists!\n";
    }
}

// Resume a created screen by name
void ScreenManager::resumeScreen(const string& screenName) {
    if (screenMap.find(screenName) != screenMap.end()) {
        currentScreenIndex = screenMap[screenName];
        vectorScreen[currentScreenIndex]->redrawScreen();  // Redraw the screen when resuming
    }
    else {
        cout << "No screen with the name '" << screenName << "' found.\n";
    }
}

// Switch back to the main screen
void ScreenManager::switchToMainScreen() {
    currentScreenIndex = 0;  // Main screen is always at index 0
    system("cls");
    vectorScreen[currentScreenIndex]->redrawScreen();
}

// Adds content to the list of commands
void ScreenManager::addContent(const string& content) { 
    vectorScreen[currentScreenIndex]->Store(content);
}

// Handles current command of the screens
void ScreenManager::handleCurrentCommand(const string& command) {
    vectorScreen[currentScreenIndex]->handleCommand(command);
}

// Check if the main screen has requested an exit
bool ScreenManager::isMainScreenExitRequested() {
    return dynamic_cast<MainScreen*>(vectorScreen[0].get())->isExitRequested();
}

void ScreenManager::fcfsRunScheduler(FCFS_Scheduler& schedulerMain, vector<Process>& processList, vector<CPU_Core>& cpuList) {
    fcfsThread = thread ([&schedulerMain, &processList, &cpuList]() {
        cout << "Starting the scheduler...\n"; // Debugging line
        schedulerMain.runScheduler(processList, cpuList);
    });
}

void ScreenManager::fcfsStopScheduler() {
    return schedulerFCFS.stopScheduler();
}
