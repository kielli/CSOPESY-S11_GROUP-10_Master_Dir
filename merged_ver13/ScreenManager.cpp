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
    this->isInitialized = true;
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
string ScreenManager::printConfig() const {
    stringstream ss;
    ss << "\nLIST OF CONFIGURATION\n";
    ss << "Number of CPU: " << configData.numCPU
        << "\nScheduler: " << configData.scheduler
        << "\nQuantum cycles: " << configData.quantumCycles
        << "\nBatch process frequency: " << configData.batchProcessFreq
        << "\nMin instructions: " << configData.minInstructions
        << "\nMax instructions: " << configData.maxInstructions
        << "\nDelay per execution: " << configData.delayPerExec
        << "\n=====================================================" << endl;
    return ss.str();
}

void ScreenManager::initializeConfigEntries() {
    this->readFile("config.txt");
    this->addContent("Program initialized...\n");

    this->initialize(this->getNumCPU(), this->getScheduler(), this->getQuantumCycles(),
                    this->getBatchProcessFreq(), this->getMinInstructions(),
                    this->getMaxInstructions(), this->getDelayPerExec());

    this->addContent(this->printConfig());
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

void ScreenManager::setCpuCounterCycle(int cpuCycle) {
    this->cpuCycleCounter = cpuCycle;
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

// Getter functions for each config value
int ScreenManager::getNumCPU() const { 
    return configData.numCPU;
}

string ScreenManager::getScheduler() const {
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

KeyboardEventHandler ScreenManager::getKEH() {
    return this->KEH;
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

vector<CPU>& ScreenManager::get_cpuList() {
    return this->fscheduler.get_cpuList();
}

FCFS_Scheduler& ScreenManager::getFCFSScheduler() {
    return this->fscheduler;
}

RR_Scheduler& ScreenManager::getRRscheduler() {
    return this->rrScheduler;
}

bool ScreenManager::getInitializeState() {
    return this->isInitialized;
}