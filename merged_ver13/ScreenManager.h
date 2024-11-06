#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Screen.h"
#include "MainScreen.h"
#include "CreatedScreen.h"
#include "KeyboardEventHandler.h"
#include "FCFS_Scheduler.h"
#include "RR_Scheduler.h"
#include "CPU.h"
#include "SchedulerManager.h"

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

using namespace std;

class ScreenManager {
    private:
        KeyboardEventHandler KEH;

        vector<unique_ptr<Screen>> screens;
        unordered_map<string, int> screenMap;

        int currentScreenIndex = 0; 
        bool isInitialized = false;

        string scheduler = "fcfs";
        int numCPU = 4;
        int quantumCycles = 4;
        int batchProcessFreq = 0;
        int minInstructions = 0;
        int maxInstructions = 0;
        int delayPerExec = 0;
        int cpuCycleCounter = 0;

        FCFS_Scheduler fscheduler = FCFS_Scheduler(this->numCPU, this->cpuCycleCounter, this->delayPerExec);
        RR_Scheduler rrScheduler;
        
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

    public:
        // Struct that holds a pointer to a string (key), and a pointer to the setter function (setter)
        struct configEntries {
            const char* key;                  // Key in the config file
            void (ScreenManager::* setter)(const string&); // Setter function for the key
        };

        ScreenManager();

        vector<CPU>& get_cpuList();
        vector<Process>& getProcessList();
        vector<Screen>& getScreens();
        static vector<configEntries> configMap(); // Static function to map config keys to setter functions

        KeyboardEventHandler getKEH();
        FCFS_Scheduler& getFCFSScheduler();
        RR_Scheduler& getRRscheduler();

        void initialize(int numCPU, string scheduler, int quantumCycles, int batchProcessFreq, int minInstructions, int maxInstructions, int delayPerExec);
        void initializeConfigEntries();

        void createScreen(const string& screenName);
        void createDummyScreen(const string& screenName, const int instructionCount);
       
        void resumeScreen(const string& screenName);
        void switchToMainScreen();  
        void displayCurrentScreen();

        bool isMainScreenExitRequested();

        void addContent(const string& content);
        void handleCurrentCommand(const string& command);
        void PollKeyboard(KeyboardEventHandler KEH);

        void readFile(const string&); // Function to read configuration from a file
        string printConfig() const; // Function to print configuration in a human-readable format

        void setCpuCounterCycle(int cpuCycle);

        // Setter functions for each config value
        void setNumCPU(const string&);
        void setScheduler(const string&);
        void setQuantumCycles(const string&);
        void setBatchProcessFreq(const string&);
        void setMinInstructions(const string&);
        void setMaxInstructions(const string&);
        void setDelayPerExec(const string&);

        // Getter functions for the configuration values
        int getNumCPU() const;
        string getScheduler() const;
        int getQuantumCycles() const;
        int getBatchProcessFreq() const;
        int getMinInstructions() const;
        int getMaxInstructions() const;
        int getDelayPerExec() const;

        int getTotalProcess();
        int getCpuCounterCycle();
        int getCurrentScreenIndex();
        bool getInitializeState();
};

#endif