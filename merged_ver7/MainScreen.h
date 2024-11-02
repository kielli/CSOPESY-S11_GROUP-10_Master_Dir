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
    int screenCount = 0;
    bool schedulerStop = false;

    ScreenManager* screenManager;  // pointer to the screen manager
    unordered_map<string, function<void(const string&)>> commands;

public:
    virtual ~MainScreen() {};
    // constructor to initialize the screen manager
    MainScreen(ScreenManager* manager);

    // displays the main screen
    void display() override;

    void displayHeader();

    // handles commands input from the user from main screen
    void handleCommand(const string& command) override;

    // checks if the exit command is requested
    bool isExitRequested() const;

    // returns the count of screens
    int countScreens() const;
    void runSchedulerTest();
  /*  void runSchedulerFCFS();*/
    /*void runSchedulerTest(ScreenManager* screenManager, atomic<bool>& KeyPress, atomic<bool>& schedulerStop);*/
};

#endif