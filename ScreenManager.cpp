#include <iostream>
#include <thread>

#include "ScreenManager.h"

// constructor for ScreenManager, always adds the main screen at index 0
ScreenManager::ScreenManager() {
    // add the main screen at index 0
    screens.push_back(make_unique<MainScreen>(this));
    currentScreenIndex = 0;  // Set the current screen to the main screen
}

// Create a new named screen
void ScreenManager::createScreen(const string& screenName) {
    system("cls");
    if (screenMap.find(screenName) == screenMap.end()) {
        screens.push_back(std::make_unique<CreatedScreen>(screenName, this));  // pass the ScreenManager pointer to CreatedScreen
        screenMap[screenName] = screens.size() - 1;  // Map screen name to index
        currentScreenIndex = screens.size() - 1;  // Switch to the new screen
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

// Display the current screen
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
void ScreenManager::createCores() {

    cpuList.clear();

    for (int i = 0; i < cpunum; ++i) {
        cpuList.push_back(CPU_Core(i));
        cout << "Debugging Core: " << i << endl; // Debugging line
    }
}
void ScreenManager::createProcess() {
    //randomized number of instruction based on max and min instructions
    int range = maxIn - minIn + 1;
    int num = rand() % range + minIn;
    processList.clear();
    //batchfreq used to determine number of process
    for (int i = 0; i < batchfreq; ++i) {
        processList.push_back(Process("process_" + std::to_string(i), i, num));
        cout << "Debugging Created: process_" << i << endl; // Debugging line
    }
}

void ScreenManager::handleCurrentCommand(const string& command) {
    if (!initialized) {
        if (command == "initialize") {
            initialized = true;
            ifstream f("config.txt");
            cpunum = 2; quantum = 2; batchfreq = 2; minIn = 2; maxIn = 2;
            schedType = "RR";
            createCores();
            createProcess();
            screens[currentScreenIndex]->Store("Program initialized \n");
            cout << "Number of CPUs: " << cpunum << "\n";
            cout << "Scheduler Type: " << schedType << "\n";
            cout << "Quantum Cycles: " << quantum << "\n";
            cout << "Batch Process Frequency: " << batchfreq << "\n";
            cout << "Minimum Instructions: " << minIn << "\n";
            cout << "Maximum Instructions: " << maxIn << "\n";
            cout << "Delays per Execution: " << delay << "\n";
            
            /*
            if (!f.is_open()) {
                screens[currentScreenIndex]->Store("File not found\n");
                //initialized = false;
            }
            else {
                f >> cpunum >> schedType >> quantum >> batchfreq >> minIn >> maxIn >> delay;
                createCores(cpunum);
                screens[currentScreenIndex]->Store("Program initialized \n");
                cout << "Number of CPUs: " << cpunum << "\n";
                cout << "Scheduler Type: " << schedType << "\n";
                cout << "Quantum Cycles: " << quantum << "\n";
                cout << "Batch Process Frequency: " << batchfreq << "\n";
                cout << "Minimum Instructions: " << minIn << "\n";
                cout << "Maximum Instructions: " << maxIn << "\n";
                cout << "Delays per Execution: " << delay << "\n";
            }
            */

        }
        else if (command == "exit") {
            screens[currentScreenIndex]->handleCommand(command);
            switchToMainScreen();
        }
        else {
            screens[currentScreenIndex]->Store ("Program uninitialized \n");
        }
    }
    else {
        screens[currentScreenIndex]->handleCommand(command);

        // if exiting from a created screen, switch to the main screen
        if (dynamic_cast<CreatedScreen*>(screens[currentScreenIndex].get()) && command == "exit") {
            switchToMainScreen();
        }
    }
}

void ScreenManager::PollKeyboard(KeyboardEventHandler KEH) {
    string commandInput;
    getline(std::cin, commandInput);
    this->screens[this->currentScreenIndex]->handleCommand(commandInput);
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Adjust polling rate
}

void ScreenManager::runScheduler() {
    schedulerThreadR = std::thread([this]() {
        rscheduler.runScheduler(processList, cpuList);
    });
    
}