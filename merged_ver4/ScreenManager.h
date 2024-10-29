#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Screen.h"
#include "CreatedScreen.h"
#include "CPU_Core.h"
#include "Process.h"
#include "FCFS_Scheduler.h"

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

using namespace std;

class ScreenManager {
    private:
        vector<unique_ptr<Screen>> vectorScreen; // screen vector with unique pointer assignment
        unordered_map<string, int> screenMap; // maps a screen name and its index
        
        int currentScreenIndex = 0;  // current screen index

        FCFS_Scheduler schedulerFCFS;
        FCFS_Scheduler* fcfsPtr;
        thread fcfsThread;
        
    public:
        ScreenManager();

        void createScreen(const string& screenName);
        void resumeScreen(const string& screenName);
        void switchToMainScreen();  
        void displayCurrentScreen();

        void addContent(const string& content);
        void handleCurrentCommand(const string& command);
        string commandInput(string& command); 
        string commandScheduler(string& command); 

        bool isMainScreenExitRequested();

        void fcfsRunScheduler(FCFS_Scheduler& scheduler, vector<Process>& processes, vector<CPU_Core>& cores);
        void fcfsStopScheduler();
};

#endif