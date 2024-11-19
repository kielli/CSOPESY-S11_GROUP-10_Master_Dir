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

    for (int i = 0; i < this->numCPU; i++) {
        this->fcfsScheduler.get_cpuList().push_back(CPU(i));
        this->rrScheduler.get_cpuList().push_back(CPU(i));
    }
}

FCFS_Scheduler& ScreenManager::getFCFSScheduler() {
    return this->fcfsScheduler;
}

RR_Scheduler& ScreenManager::getRRscheduler() {
    return this->rrScheduler;
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
    this->initializedState = true;
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

// Create a new named screen
void ScreenManager::createScreen(const string& screenName,  int instructionCount = 0)
{
    system("cls");
    int newPID = this->getTotalProcess();

    if (screenMap.find(screenName) == screenMap.end()) {
        // pass the ScreenManager pointer to CreatedScreen
        screens.push_back(make_unique<CreatedScreen>(screenName, this, newPID, instructionCount, this->memPerProc));
        
        screenMap[screenName] = screens.size() - 1;  // map screen name to index
        currentScreenIndex = screenMap[screenName]; // switch to new screen

        screens[0]->setActiveStatusOff();
        screens[currentScreenIndex]->setActiveStatusOn();

        if(this->scheduler == "\"fcfs\"")
            this->fcfsScheduler.getProcessList().push_back(*screens.back());
        else if(this->scheduler == "\"rr\"")
            this->rrScheduler.getProcessList().push_back(*screens.back());

        // debugging line
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

        if(this->scheduler == "\"fcfs\"")
            this->fcfsScheduler.getProcessList().push_back(*screens.back());
        else if(this->scheduler == "\"rr\"")
            this->rrScheduler.getProcessList().push_back(*screens.back());
        
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

void ScreenManager::addContent(const string& content) {
    screens[currentScreenIndex]->Store(content);
}

void ScreenManager::initializeConfigEntries() {
    this->readFile("config.txt");
    this->addContent("Program initialized...\n");

    this->initialize(configData.numCPU, configData.scheduler, configData.quantumCycles,
                configData.batchProcessFreq, configData.minInstructions,
                configData.minInstructions, configData.delayPerExec);
                
    this->addContent(this->printConfig());
}

void ScreenManager::initializeCommand()
{
    string commandInput;
    cout << "Enter a command: ";
    getline(cin, commandInput);

    if (commandInput == "initialize") {
        this->initializeConfigEntries();
        this->initializedState = true;
    }
    else if (commandInput == "exit") {
        addContent("Exiting program...\n");
        exit(0);
    }
    else {
        this->invalidCommand(commandInput);
        this->addContent("'initialize' the program first.\n");
    }
}

void ScreenManager::invalidCommand(const string& command) {
    screens[currentScreenIndex]->print_error(command);
}

void ScreenManager::handleCurrentCommand(const string& command)
{    
    if(command.find("screen") == 0)
    {
        if(currentScreenIndex == 0)
        {
            stringstream iss(command.substr(6));
            string option, screenName;
            iss >> option >> screenName;

            if (option == "-s" && !screenName.empty()) {
                screens[currentScreenIndex]->printAndStore("\nsreen -s " + screenName + " command recognized. Doing something.");
                this->createScreen(screenName);
            }
            else if (option == "-r" && !screenName.empty()) {
                screens[currentScreenIndex]->printAndStore("\nsreen -r " + screenName + " command recognized. Doing something.");
                this->resumeScreen(screenName);
            }
            else if (option == "-ls") {
                if(this->scheduler == "\"fcfs\"") {
                    this->addContent("\n[DEBUG] Displaying FCFS Processes.");
                    this->addContent(this->fcfsScheduler.displayProcesses());
                }
                else if(this->scheduler == "\"rr\"") {
                    this->addContent("\n[DEBUG] Displaying RR Processes.");
                    this->addContent(this->rrScheduler.displayProcesses());
                }
            }
            else {
                this->invalidCommand(command);
            }
        }
        else {
             this->invalidCommand(command);
        }
    }
    else if (command.find("scheduler") == 0)
    {
        if (currentScreenIndex == 0) 
        {
            stringstream iss(command.substr(9));
            string option;
            iss >> option;

            if (option == "-test") {
                // always set the schedulerStop to false
                schedulerStop = false;
                
                screens[currentScreenIndex]->printAndStore("scheduler -test command recognized. Doing something.");
                this->runSchedulerTest();
            }
            else if (option == "-stop") {
                screens[currentScreenIndex]->printAndStore("scheduler -stop command recognized. Doing something.");
                schedulerStop = true;
            }
            else {
                this->invalidCommand(command);
            }
        }
        else {
            this->invalidCommand(command);
        }
    }
    else if (command == "process -smi")
    {
        if (this->currentScreenIndex != 0) {
            this->screens[currentScreenIndex]->displayProcessSmi();
        }
        else {
            this->invalidCommand(command);
        }
    }
    else if (command == "report -util")
    {
        if (this->currentScreenIndex == 0)
        {
            ofstream reportfile;

            reportfile.open("csopesy-log.txt");
            if (!reportfile.is_open()) {
                cerr << "Unable to open file." << endl;
            }

            streambuf* cout_buffer = cout.rdbuf(); 
            cout.rdbuf(reportfile.rdbuf());

           if(this->scheduler == "\"fcfs\"")
                this->addContent(this->fcfsScheduler.displayProcesses());
            else if(this->scheduler == "\"rr\"")
                this->addContent(this->rrScheduler.displayProcesses());

            cout.rdbuf(cout_buffer); 
            reportfile.close();

            cout << "Report generated at csopesy-log.txt.\n";
        }
        else {
            this->invalidCommand(command);
        }
    }
    else if (command == "clear")
    {
        if (currentScreenIndex == 0) {
            screens[0]->deleteContent();
            screens[0]->display();
        }
        else {
            this->invalidCommand(command);
        }
        
    }
    else if (command == "exit")
    {
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
            cout << "Exiting the program...\n";
            exit(0);
        }
    }
    else {
        this->invalidCommand(command);
    }
}

void ScreenManager::runSchedulerTest()
{
    int counter = this->getTotalProcess();
    int cpuCycleCounter = 0;

    while (schedulerStop == false)
    {
        if (_kbhit()) {
            this->PollKeyboard();
            this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        if (cpuCycleCounter % 4 == 0) {
            string pname = "process" + to_string(counter);
            srand(static_cast<unsigned>(time(0) + 39 % 4));
            int instructionCount = this->getMinInstructions() + rand() % (/*screenManager->getMaxInstructions() - screenManager->getMinInstructions()*/ 100 - 200 + 1);

            this->createDummyScreen(pname, instructionCount);
            counter++;
        }
        this_thread::sleep_for(std::chrono::milliseconds(50));
        cpuCycleCounter++;
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
