#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Screen.h"
#include "MainScreen.h"
#include "CreatedScreen.h"
#include "CPU.h"
#include "FCFS_Scheduler.h"
#include "RR_Scheduler.h"

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

using namespace std;

class ScreenManager {
    private:
        vector<unique_ptr<Screen>> screens;
        unordered_map<string, int> screenMap;

        int currentScreenIndex = 0; 
        bool schedulerStop = false;
        bool initializedState = false;

        string scheduler = "";
        int numCPU = 0;
        int quantumCycles = 0;
        int batchProcessFreq = 0;
        int minInstructions = 0;
        int maxInstructions = 0;
        int delayPerExec = 0;
        int cpuCycleCounter = 0;

        size_t maxOverallMem = 0;
        size_t memPerFrame = 0;
        size_t memPerProc = 0;  

        // Struct that holds the configuration settings
        struct configs {
            int numCPU;
            string scheduler;
            int quantumCycles;
            int batchProcessFreq;
            int minInstructions;
            int maxInstructions;
            int delayPerExec;
        };

        configs configData;  // Holds the configuration values

        FCFS_Scheduler fcfsScheduler = FCFS_Scheduler(this->numCPU, this->cpuCycleCounter, this->delayPerExec);
        RR_Scheduler rrScheduler;

    public:
        ScreenManager();

         // Struct that holds a pointer to a string (key), and a pointer to the setter function (setter)
        struct configEntries {
            const char* key;                  // Key in the config file
            void (ScreenManager::* setter)(const string&); // Setter function for the key
        };

        static vector<configEntries> configMap(); // Static function to map config keys to setter functions

        FCFS_Scheduler& getFCFSScheduler();
        RR_Scheduler& getRRscheduler();
        
        void readFile(const string& fileName);
        string printConfig() const;
        void initialize(int numCPU, string scheduler, int quantumCycles, int batchProcessFreq, int minInstructions, int maxInstructions, int delayPerExec);

        void initializeCommand();
        void invalidCommand(const string& command);

        void runSchedulerTest();
        void initializeScheduler();

        void initializeConfigEntries();

        void createScreen(const string& screenName, int instructionCount);
        void createDummyScreen(const string& screenName, const int instructionCount);
        void resumeScreen(const string& screenName);

        void switchToMainScreen();  
        void displayCurrentScreen();
        void addContent(const string& content);

        bool isMainScreenExitRequested();
        bool getInitializedState();

        void handleCurrentCommand(const string& command);
        void PollKeyboard();

        void setCpuCounterCycle(int cpuCycle);

        // Getter functions for the configuration values
        string getSchedulerType() const;

        int getNumCPU() const;
        int getQuantumCycles() const;
        int getBatchProcessFreq() const;
        int getMinInstructions() const;
        int getMaxInstructions() const;
        int getDelayPerExec() const;

        int getTotalProcess();
        int getCpuCounterCycle();
        int getCurrentScreenIndex();

        // Setter functions for each config value
        void setNumCPU(const string&);
        void setScheduler(const string&);
        void setQuantumCycles(const string&);
        void setBatchProcessFreq(const string&);
        void setMinInstructions(const string&);
        void setMaxInstructions(const string&);
        void setDelayPerExec(const string&);
};

#endif