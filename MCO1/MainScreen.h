#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "Screen.h"

#include <unordered_map>
#include <functional>
#include <string>

using namespace std;

class ScreenManager;

class MainScreen : public Screen {
    private:
        bool isExit = false;
        bool schedulerStop = false;
        int screenCount = 0;

        ScreenManager* screenManager;  // pointer to the screen manager
        unordered_map<string, function<void(const string&)>> commands;

    public:
        virtual ~MainScreen() {};
        MainScreen(ScreenManager* manager); // constructor to initialize the screen manager

        void display() override;    // displays the main screen
        void handleCommand(const string& command) override; // handles commands input from the user from main screen

        bool isExitRequested() const;   // checks if the exit command is requested

        int countScreens() const; // returns the count of screens

        void displayHeader();
        void runSchedulerTest();
};

#endif