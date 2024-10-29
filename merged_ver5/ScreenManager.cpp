#include "ScreenManager.h"
#include <iostream>
#include <thread>
#include <chrono>

// Constructor for ScreenManager, always adds the main screen at index 0
ScreenManager::ScreenManager() {
    screens.push_back(make_unique<MainScreen>(this)); // Add the main screen at index 0
    currentScreenIndex = 0;  // Set the current screen to the main screen
}

// Initializes config.txt variables
void ScreenManager::initialize(int numCPU, string scheduler, int quantumCycles, int batchProcessFreq, int minInstructions, int maxInstructions, int delayPerExec)
{
    this->numCPU = numCPU;
    this->scheduler = scheduler;
    this->batchProcessFreq = batchProcessFreq;
    this->quantumCycles = quantumCycles;
    this->minInstructions = minInstructions;
    this->maxInstructions = minInstructions;
    this->delayPerExec = delayPerExec;
    this->initialized = true;
}

// Displays the current screen
void ScreenManager::displayCurrentScreen() {
    screens[currentScreenIndex]->display();
}

// Create a new named screen
void ScreenManager::createScreen(const std::string& screenName) {
    system("cls");
    if (screenMap.find(screenName) == screenMap.end()) {
        screens.push_back(std::make_unique<CreatedScreen>(screenName, this));  // pass the ScreenManager pointer to CreatedScreen
        screenMap[screenName] = screens.size() - 1;  // map screen name to index
        currentScreenIndex = screens.size() - 1;  // switch to the new screen
        screens[0]->setActiveStatusOff();
        screens[currentScreenIndex]->setActiveStatusOn();
    }
    else {
        cout << "Screen with name '" << screenName << "' already exists!\n";
    }
}

// Creates a dummy screen
void ScreenManager::createDummyScreen(const std::string& screenName) {
    if (screenMap.find(screenName) == screenMap.end()) {
        screens.push_back(std::make_unique<CreatedScreen>(screenName, this));  // pass the ScreenManager pointer to CreatedScreen
        screenMap[screenName] = screens.size() - 1;  // map screen name to index
    }
    else {
        cout << "Screen with name '" << screenName << "' already exists!\n";
    }
}

// Resume a created screen by name
void ScreenManager::resumeScreen(const string& screenName) {
    if (screenMap.find(screenName) != screenMap.end()) {
        screens[currentScreenIndex]->setActiveStatusOff();
        currentScreenIndex = screenMap[screenName];
        screens[currentScreenIndex]->setActiveStatusOn();
        screens[currentScreenIndex]->redrawScreen();  // Redraw the screen when resuming
    }
    else {
        cout << "No screen with the name '" << screenName << "' found.\n";
    }
}

// Switch back to the main screen
void ScreenManager::switchToMainScreen() {
    screens[currentScreenIndex]->setActiveStatusOff();
    currentScreenIndex = 0;  // Main screen is always at index 0
    screens[0]->setActiveStatusOn();
    system("cls");
    screens[currentScreenIndex]->redrawScreen();
}

// Add content and store it
void ScreenManager::addContent(const string& content) {
    screens[currentScreenIndex]->Store(content);
}

// Handles current command of the screens
void ScreenManager::handleCurrentCommand(const string& command) {
    screens[currentScreenIndex]->handleCommand(command);

    // if exiting from a created screen, switch to the main screen
    if (dynamic_cast<CreatedScreen*>(screens[currentScreenIndex].get()) && command == "exit") {
        switchToMainScreen();
    }
}

KeyboardEventHandler ScreenManager::getKEH() {
    return this->KEH;
}

void ScreenManager::PollKeyboard(KeyboardEventHandler KEH) {
    string commandInput;
    getline(std::cin, commandInput);
    this->screens[this->currentScreenIndex]->handleCommand(commandInput);
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Adjust polling rate
}

int ScreenManager::getTotalProcess() {
    return this->screens.size();
}

int ScreenManager::getCpuCounterCycle() {
    return this->cpuCycleCounter;
}

void ScreenManager::setCpuCounterCycle(int cpuCycle) {
    this->cpuCycleCounter = cpuCycle;
}

int ScreenManager::getbatchProcessFreq() {
    return this->batchProcessFreq;
}

// Check if the main screen has requested an exit
bool ScreenManager::isMainScreenExitRequested() {
    return dynamic_cast<MainScreen*>(screens[0].get())->isExitRequested();
}

// Returns initialize state
bool ScreenManager::getInitializeState() {
    return this->initialized;
}