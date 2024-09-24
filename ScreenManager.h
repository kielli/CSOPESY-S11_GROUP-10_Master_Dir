#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <map>
#include <string>

#include "Screen.h"

class ScreenManager {
public:
    void createScreen(std::string name);
    void resumeScreen(std::string name);
    //void listScreens();
    Screen* getScreen(std::string name); // New method to get a screen by name

private:
    int currentScreen;
    std::map<std::string, Screen> screens;
};

#endif // SCREENMANAGER_H
