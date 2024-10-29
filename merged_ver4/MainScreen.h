#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "Screen.h"
#include "ScreenManager.h"
#include "Process.h"
#include "CPU_Core.h"
#include "FCFS_Scheduler.h"

#include <string>
#include <functional>
#include <unordered_map>

using namespace std;

class ScreenManager; 

class MainScreen : public Screen {
    private:
        ScreenManager* screenManager;  // pointer to the screen manager
        bool isExit = false;
        unordered_map<string, function<void(const string&)>> commandMap; // command map that stores commands

        vector<Process> processList;
        vector<CPU_Core> cpuList;
        FCFS_Scheduler schedulerFCFS;

    public:
        MainScreen(ScreenManager* manager);
        
        void displayScreen() override;
        void handleCommand(const string& command)override;
 
        void displayHeader();
        void readFile();
        
        void createProcesses();
        void createCores();

        bool isExitRequested() const;
};

#endif