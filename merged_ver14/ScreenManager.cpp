#include "ScreenManager.h"
#include "FCFS_Scheduler.h"
#include "RR_Scheduler.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <conio.h>

// Constructor for ScreenManager, always adds the main screen at index 0
ScreenManager::ScreenManager() {
    screens.push_back(make_unique<MainScreen>(this));
    currentScreenIndex = 0; 
}

// Define static map of config keys and their setter functions
vector<ScreenManager::configEntries> ScreenManager::configMap() {
    return {
        {"num-cpu", &ScreenManager::setNumCPU},
        {"scheduler", &ScreenManager::setScheduler},
        {"quantum-cycles", &ScreenManager::setQuantumCycles},
        {"batch-process-freq", &ScreenManager::setBatchProcessFreq},
        {"min-ins", &ScreenManager::setMinInstructions},
        {"max-ins", &ScreenManager::setMaxInstructions},
        {"delay-per-exec", &ScreenManager::setDelayPerExec}
    };
}

// Reads the configuration from a file and applies the settings
void ScreenManager::readFile(const string& fileName) {
    string line;
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cerr << "Unable to open file " << fileName << endl;
        return;
    }

    // Loop through each line of the config file
    while (getline(inputFile, line)) {
        // Check each config entry to see if it matches the line
        for (const auto& configEntry : configMap()) {
            if (line.find(configEntry.key) != string::npos) {
                // Extract the value after the key and call the corresponding setter function
                (this->*configEntry.setter)(line.substr(line.find(" ") + 1));
                break; // Stop after finding a matching key
            }
        }
    }

    inputFile.close();
}

// Converts the configuration into a human-readable string
void ScreenManager::printConfig() const {
    cout << "\nLIST OF CONFIGURATION\n";

    cout << "Number of CPU: " << configData.numCPU;
    cout << "\nScheduler: " << configData.scheduler;
    cout << "\nQuantum cycles: " << configData.quantumCycles;
    cout << "\nBatch process frequency: " << configData.batchProcessFreq;
    cout << "\nMin instructions: " << configData.minInstructions;
    cout << "\nMax instructions: " << configData.maxInstructions;
    cout << "\nDelay per execution: " << configData.delayPerExec;
    cout << "\n=====================================================";
}

// Create a new named screen
void ScreenManager::createScreen(const string& screenName,  int instructionCount = 0)
{
    system("cls");
    int ptrNumber = this->getTotalProcess();

    if (screenMap.find(screenName) == screenMap.end()) {
        // pass the ScreenManager pointer to CreatedScreen
        screens.push_back(make_unique<CreatedScreen>(screenName, this, ptrNumber, this->getMaxInstructions(), this->memPerProc));
        
        screenMap[screenName] = screens.size() - 1;  // map screen name to index
        currentScreenIndex = screenMap[screenName]; // switch to new screen
        screens[0]->setActiveStatusOff();
        screens[currentScreenIndex]->setActiveStatusOn();

        this->schedulerMain->getProcessList().push_back(*screens.back());

         cout << screenName << "' created with " << instructionCount  << " instructions." << "\n";
    }
    else {
        cout << "Screen with name '" << screenName << "' already exists!\n";
    }
}

void ScreenManager::createDummyScreen(const string& screenName, const int instructionCount) {
    int newPID = this->getTotalProcess();

    if (screenMap.find(screenName) == screenMap.end()) {
        // pass the ScreenManager pointer to CreatedScreen
        screens.push_back(make_unique<CreatedScreen>(screenName, this, newPID, instructionCount, this->memPerProc)); 
        screenMap[screenName] = screens.size() - 1;  // map screen name to index

        this->schedulerMain->getProcessList().push_back(*screens.back());
        
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
        screens[currentScreenIndex]->display();
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
    screens[currentScreenIndex]->display();
}

// display the current screen
void ScreenManager::displayCurrentScreen() {
    screens[currentScreenIndex]->display();
}

// Check if the main screen has requested an exit
bool ScreenManager::isMainScreenExitRequested() {
    return dynamic_cast<MainScreen*>(screens[0].get())->isExitRequested();
}

bool ScreenManager::getInitializedState() {
    return this->initializedState;
}

void ScreenManager::initializeConfigEntries() {
    this->readFile("config.txt");

    cout << "Program initialized...\n";

    this->scheduler = configData.scheduler;
    this->numCPU = configData.numCPU;
    this->quantumCycles = configData.quantumCycles;
    this->batchProcessFreq = configData.batchProcessFreq;
    this->minInstructions = configData.minInstructions;
    this->maxInstructions = configData.maxInstructions;
    this->delayPerExec = configData.delayPerExec;

    this->printConfig();
}

void ScreenManager::initializeCommand()
{
    this->initializeConfigEntries();
    this->initializedState = true;

    if (this->scheduler == "\"fcfs\"") {
        FCFS_Scheduler fcfsSched(this->numCPU, this->cpuCycleCounter, this->delayPerExec, this->quantumCycles);
        this->schedulerMain = &fcfsSched;
        
        // debugging line
        cout << "\nSet to FCFS Scheduler.\n";
    }
    else if(this->scheduler == "\"rr\"") {
        RR_Scheduler rrSched(this->numCPU, this->cpuCycleCounter, this->delayPerExec, this->quantumCycles);
        this->schedulerMain = &rrSched;

        // debugging line
        cout << "\nSet to RR Scheduler.\n";
    }

    // debugging line
    cout << "Initialized state: " << this->getInitializedState() << endl;
}

void ScreenManager::handleCurrentCommand(const string& command)
{
    if (command.find("screen") == 0)
    {
        if (currentScreenIndex == 0)
        {
            stringstream iss;
            string option, screenName;
            iss >> option >> screenName;

            if (option == "-s" && !screenName.empty()) {
                this->createScreen(screenName);
            }
            else if (option == "-r" && !screenName.empty()) {
                this->resumeScreen(screenName);
            }
            else if (option == "-ls") {
                this->schedulerMain->displayProcesses();
            }
            else {
                cout << "Invalid command.\n";
            }
        }
        else {
            cout << "Invalid command.\n";
        }
    }
    else if (command.find("scheduler") == 0 )
    {
        if (currentScreenIndex == 0) 
        {
            stringstream iss;
            string option;
            iss >> option;

            if (option == "-test")
            {
                screens[currentScreenIndex]->printAndStore("scheduler -test command recognized. Doing something.");

                random_device rd;
                mt19937 gen(rd());
                uniform_int_distribution<> dis(this->minInstructions, this->maxInstructions);

                int cycleCounter = 0;

                while (schedulerStop == false) {
                    if (_kbhit()) {
                        this->PollKeyboard();
                        this_thread::sleep_for(std::chrono::milliseconds(10));
                        continue;
                    }

                    if (cycleCounter % this->batchProcessFreq == 0) {
                        string pname = "process" + to_string(this->screens.size() - 1);
                        int randomNum = dis(gen);
                        this->createDummyScreen(pname, randomNum);
                    }
                    cpuCycleCounter++;
                }
            }
            else if (option == "-stop") {
                screens[currentScreenIndex]->printAndStore("scheduler -test command recognized. Doing something.");
                schedulerStop = true;
            }
            else {
                cout << "Invalid command.\n";
            }
        }
        else {
            cout << "Invalid command.\n";
        }
    }
    else if (command == "clear") {
        if (currentScreenIndex == 0) {
            screens[0]->deleteContent();
            screens[0]->display();
        }
        else {
            cout << "Invalid command.\n";
        }
        
    }
    else if (command == "exit") {
        if (this->currentScreenIndex != 0)
        {
            this->switchToMainScreen();
            this->screens[currentScreenIndex]->setRunningFalse();

            if (screens[currentScreenIndex]->getFinishState()) {
                screenMap.erase(screens[currentScreenIndex]->getPName());
                screens.erase(screens.begin() + currentScreenIndex);
            }
        }
        else {
            cout << "Exiting program...\n";
            exit(0);
        }
    }
    else if (command == "process -smi") {
        if (this->currentScreenIndex != 0) {
            this->screens[currentScreenIndex]->displayProcessSmi();
        }
        else {
            cout << "Invalid command.\n";
        }
    }
    else if (command == "report -util") {
        if (this->currentScreenIndex == 0)
        {
            ofstream reportfile;

            reportfile.open("csopesy-log.txt");
            if (!reportfile.is_open()) {
                cerr << "Unable to open file." << endl;
            }

            streambuf* cout_buffer = cout.rdbuf(); 
            cout.rdbuf(reportfile.rdbuf());
            this->schedulerMain->displayProcesses();

            cout.rdbuf(cout_buffer); 
            reportfile.close();

            cout << "Report generated at csopesy-log.txt.\n";
        }
        else {
            cout << "Invalid command.\n";
        }
    }
    else {
        cout << "Invalid command.\n";
    }
}

void ScreenManager::PollKeyboard() {
    string commandInput;
    getline(cin, commandInput);
    this->handleCurrentCommand(commandInput);
    this_thread::sleep_for(chrono::milliseconds(10)); // Adjust polling rate
}

void ScreenManager::setCpuCounterCycle(int cpuCycle) {
    this->cpuCycleCounter = cpuCycle;
}

// Getter functions for each config value
int ScreenManager::getNumCPU() const { 
    return configData.numCPU;
}

string ScreenManager::getSchedulerType() const {
    return configData.scheduler;
}

int ScreenManager::getQuantumCycles() const {
    return configData.quantumCycles;
}

int ScreenManager::getBatchProcessFreq() const {
    return configData.batchProcessFreq;
}

int ScreenManager::getMinInstructions() const {
    return configData.minInstructions;
}

int ScreenManager::getMaxInstructions() const {
    return configData.maxInstructions;
}

int ScreenManager::getDelayPerExec() const {
    return configData.delayPerExec;
}

int ScreenManager::getTotalProcess() {
    return this->screens.size();
}

int ScreenManager::getCpuCounterCycle() {
    return this->cpuCycleCounter;
}

int ScreenManager::getCurrentScreenIndex() {
    return this->currentScreenIndex;
}

Scheduler& ScreenManager::getScheduler() {
    return *schedulerMain;
}

// Setter functions for each config value
void ScreenManager::setNumCPU(const string& value) {
    configData.numCPU = stoi(value);
}

void ScreenManager::setScheduler(const string& value) {
    configData.scheduler = value;
}

void ScreenManager::setQuantumCycles(const string& value) {
    configData.quantumCycles = stoi(value);
}

void ScreenManager::setBatchProcessFreq(const string& value) {
    configData.batchProcessFreq = stoi(value);
}

void ScreenManager::setMinInstructions(const string& value) {
    configData.minInstructions = stoi(value);
}

void ScreenManager::setMaxInstructions(const string& value) {
    configData.maxInstructions = stoi(value);
}

void ScreenManager::setDelayPerExec(const string& value) {
    configData.delayPerExec = stoi(value);
}