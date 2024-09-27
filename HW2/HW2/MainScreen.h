#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "Screen.h"
#include <unordered_map>
#include <functional>
#include <string>

class ScreenManager;

class MainScreen : public Screen {
private:
    bool isExit = false;
    int screenCount = 0;
    ScreenManager* screenManager;  // pointer to the screen manager
    std::unordered_map<std::string, std::function<void(const std::string&)>> commands;

public:
    // constructor to initialize the screen manager
    MainScreen(ScreenManager* manager);

    // displays the main screen
    void display() override;

    void displayHeader();

    // handles commands input from the user from main screen
    void handleCommand(const std::string& command) override;

    // checks if the exit command is requested
    bool isExitRequested() const;

    // returns the count of screens
    int countScreens() const;
};

#endif
