#include "ScreenManager.h"
#include <iostream>
#include <cstdlib>

using namespace std;


void ScreenManager::createScreen(string name) {
    if (screens.find(name) == screens.end()) {
        Screen newConsole(name);
        screens[name] = newConsole;
        cout << "Screen session '" << name << "' created." << endl;
    }
    else {
        cout << "Screen session '" << name << "' already exists." << endl;
    }
}

void ScreenManager::resumeScreen(string name) {
    if (screens.find(name) != screens.end()) {
        cout << "Resuming screen session '" << name << "'." << endl;
    }
    else {
        cout << "No screen session found for '" << name << "'." << endl;
    }
}

Screen* ScreenManager::getScreen(std::string name) {
    if (screens.find(name) != screens.end()) {
        return &screens[name]; // Return pointer to the screen
    }
    return nullptr;
}
