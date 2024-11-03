#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Screen.h"
#include "MainScreen.h"
#include "CreatedScreen.h"
#include "KeyboardEventHandler.h"
#include "FCFS_Scheduler.h"
#include "RR_Scheduler.h"
#include "CPU.h"

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
        bool initialized = false;

        string scheduler = "fcfs";
        int numCPU = 4;
        int quantumCycles = 0;
        int batchProcessFreq = 0;
        int minInstructions = 0;
        int maxInstructions = 0;
        int delayPerExec = 0;
        int cpuCycleCounter = 0;

        FCFS_Scheduler fscheduler = FCFS_Scheduler(this->numCPU, this->cpuCycleCounter, this->delayPerExec);
        RR_Scheduler rscheduler = RR_Scheduler(this->quantumCycles);
        

    public:
        ScreenManager();

        vector<CPU>& get_cpuList();
        vector<Process>& getProcessList();
        vector<Screen>& getScreens();

        KeyboardEventHandler getKEH();
        FCFS_Scheduler& getScheduler();
        RR_Scheduler& getScheduler2();

        void initialize(int numCPU, string scheduler, int quantumCycles, int batchProcessFreq, int minInstructions, int maxInstructions, int delayPerExec);
        bool getInitializeState();

        void createScreen(const string& screenName);
        void createDummyScreen(const std::string& screenName, const int instructionCount);
       
        void resumeScreen(const string& screenName);
        void switchToMainScreen();  
        void displayCurrentScreen();

        bool isMainScreenExitRequested();

        void addContent(const string& content);
        void handleCurrentCommand(const string& command);

        void setCpuCounterCycle(int cpuCycle);

        int getTotalProcess();
        int getCpuCounterCycle();
        int getbatchProcessFreq();

        void PollKeyboard(KeyboardEventHandler KEH);

        int getMinInstructions();
        int getMaxInstructions();

        void runSchedulerFCFS();
};

#endif