#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <unordered_map>
#include <functional>
#include <string>

using namespace std;

class ScreenManager;

class MainScreen : public Screen {
    private:
        bool isExit = false;
        ScreenManager* screenManager;  // pointer to the screen manager

    public:
        virtual ~MainScreen() {};

        MainScreen(ScreenManager* manager);

        MainScreen(const string& pName, ScreenManager* manager, int pId, int numInstructions, size_t requiredMemory) 
                :   Screen(pName, manager, pId, numInstructions, requiredMemory) {}

        bool isExitRequested() const; 
        void displayHeader() override;
        void displayProcessSmi() override;
};

#endif