#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Screen.h"
#include "CreatedScreen.h"

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

using namespace std;

class ScreenManager {
    private:
        vector<unique_ptr<Screen>> screen; // screen vector with unique pointer assignment
        unordered_map<string, int> screenMap; // maps a screen name and its index
        
        int currentScreenIndex = 0;  // current screen index
        
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
};

#endif