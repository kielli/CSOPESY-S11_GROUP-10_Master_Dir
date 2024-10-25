#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "Screen.h"
#include "ScreenManager.h"

#include <string>
#include <functional>
#include <unordered_map>

using namespace std;

class ScreenManager; 

class MainScreen : public Screen {
    private:
        ScreenManager* screenManager;  // pointer to the screen manager
        bool isExit = false;
        
        // command map that stores commands
        unordered_map<string, function<void(const string&)>> commandMap;

    public:
        MainScreen();
        MainScreen(ScreenManager* manager);
        
        void displayScreen() override;
        void handleCommand(const string& command)override;
 
        void displayHeader();
        void readFile();

        bool isExitRequested() const;
        bool isExitScheduler() const;
};

#endif