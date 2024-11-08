#include "ScreenManager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>

// Constructor for ScreenManager, always adds the main screen at index 0
ScreenManager::ScreenManager() {
    screens.push_back(make_unique<MainScreen>(this));
    currentScreenIndex = 0; 

    for (int i = 0; i < this->numCPU; i++) {
        this->fscheduler.get_cpuList().push_back(CPU(i));
        this->rrScheduler.get_cpuList().push_back(CPU(i));
    }
}

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

bool ScreenManager::getInitializeState() {
    return this->initialized;
}

// Create a new named screen
void ScreenManager::createScreen(const string& screenName) {
    system("cls");
    int ptrNumber = this->getTotalProcess();

    if (screenMap.find(screenName) == screenMap.end()) {
        screens.push_back(make_unique<CreatedScreen>(screenName, this, ptrNumber, this->getMaxInstructions()));  // pass the ScreenManager pointer to CreatedScreen
        
        screenMap[screenName] = screens.size() - 1;  // map screen name to index
        currentScreenIndex = screenMap[screenName]; // switch to new screen
        
        screens[0]->setActiveStatusOff();
        screens[currentScreenIndex]->setActiveStatusOn();

        if(this->scheduler == "\"fcfs\"")
            this->fscheduler.getProcessList().push_back(*screens.back());
        else if(this->scheduler == "\"rr\"")
            this->rrScheduler.getProcessList().push_back(*screens.back());
    }
    else {
        cout << "Screen with name '" << screenName << "' already exists!\n";
    }
}

void ScreenManager::createDummyScreen(const string& screenName, const int instructionCount) {
    int ptrNumber = this->getTotalProcess();

    if (screenMap.find(screenName) == screenMap.end()) {
        screens.push_back(make_unique<CreatedScreen>(screenName, this, ptrNumber, instructionCount));  // pass the ScreenManager pointer to CreatedScreen
        
        screenMap[screenName] = screens.size() - 1;  // map screen name to index

        if(this->scheduler == "\"fcfs\"")
            this->fscheduler.getProcessList().push_back(*screens.back());
        else if(this->scheduler == "\"rr\"")
            this->rrScheduler.getProcessList().push_back(*screens.back());
      
        // for debugging
        cout << screenName << "' created with " << instructionCount << " instructions." << "\n";
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

// display the current screen
void ScreenManager::displayCurrentScreen() {
    screens[currentScreenIndex]->display();
}

// Check if the main screen has requested an exit
bool ScreenManager::isMainScreenExitRequested() {
    return dynamic_cast<MainScreen*>(screens[0].get())->isExitRequested();
}

void ScreenManager::addContent(const string& content) {
    screens[currentScreenIndex]->Store(content);
}

void ScreenManager::handleCurrentCommand(const string& command) {
    screens[currentScreenIndex]->handleCommand(command);

    // if exiting from a created screen, switch to the main screen
    if (dynamic_cast<CreatedScreen*>(screens[currentScreenIndex].get()) && command == "exit") {
        switchToMainScreen();
    }
}

void ScreenManager::PollKeyboard(KeyboardEventHandler KEH) {
    string commandInput;
    getline(cin, commandInput);
    this->screens[this->currentScreenIndex]->handleCommand(commandInput);
    this_thread::sleep_for(chrono::milliseconds(10)); // Adjust polling rate
}

KeyboardEventHandler ScreenManager::getKEH() {
    return this->KEH;
}

int ScreenManager::getTotalProcess() {
    return this->screens.size();
}

int ScreenManager::getCpuCounterCycle() {
    return this->cpuCycleCounter;
}

int ScreenManager::getbatchProcessFreq() {
    return this->batchProcessFreq;
}

int ScreenManager::getMinInstructions() {
    return this->minInstructions;
}

int ScreenManager::getMaxInstructions() {
    return this->maxInstructions;
}

int ScreenManager::getCurrentScreenIndex() {
    return this->currentScreenIndex;
}

vector<CPU>& ScreenManager::get_cpuList() {
    return this->fscheduler.get_cpuList();
}

FCFS_Scheduler& ScreenManager::getScheduler() {
    return this->fscheduler;
}

RR_Scheduler& ScreenManager::getRRscheduler() {
    return this->rrScheduler;
}

void ScreenManager::setCpuCounterCycle(int cpuCycle) {
    this->cpuCycleCounter = cpuCycle;
}