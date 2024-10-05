#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <string>
#include <functional>
#include <unordered_map>
#include "Screen.h"

using namespace std;

class ScreenManager;

class MainScreen : public Screen {
    private:
        bool isExit = false;
        int screenCount = 0;

        ScreenManager* screenManager;  // pointer to the screen manager
        unordered_map<string, function<void(const string&)>> commands;

    public:
        // constructor to initialize the screen manager
        MainScreen(ScreenManager* manager);

        // displays the main screen
        void display() override;

        void Print_nvidia_smi_Header();

        void displayHeader();

        // handles commands input from the user from main screen
        void handleCommand(const string& command) override;

        // checks if the exit command is requested
        bool isExitRequested() const;

        // returns the count of screens
        int countScreens() const;
        string getTimestamp() const;
};

#endif


