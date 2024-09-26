#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include "Screen.h"
#include "MainScreen.h"
#include "CreatedScreen.h"

class ScreenManager {
private:
    std::vector<std::unique_ptr<Screen>> screens;
    std::unordered_map<std::string, int> screenMap;
    int currentScreenIndex = 0;  // index of the current screen

public:
    ScreenManager();
    void createScreen(const std::string& screenName);
    void resumeScreen(const std::string& screenName);
    void switchToMainScreen();  // switch back to the main screen
    void displayCurrentScreen();  // display the current screen
    void displayHeader();
    bool isMainScreenExitRequested();
    void handleCurrentCommand(const std::string& command);
};

#endif
