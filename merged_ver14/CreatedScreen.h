#ifndef CREATEDSCREEN_H
#define CREATEDSCREEN_H

#include "Screen.h"
#include "ScreenManager.h"

#include <string>

using namespace std;

class ScreenManager;

class CreatedScreen : public Screen {
    private:
        string screenName;
        int pid;
        string creationTimestamp;
        size_t requiredMemory;

        ScreenManager* screenManager;  // Pointer to ScreenManager for switching screens

        bool finished = false;

    public:    
        virtual ~CreatedScreen() {};

        CreatedScreen(const string& name, ScreenManager* manager, int PID, int numInstruction, size_t requiredMemory)
            : Screen(name, manager, PID, numInstruction, requiredMemory), // Base class constructor
            screenName(name),                  // Initialize member variable
            pid(PID),                          // Initialize member variable
            creationTimestamp(this->getTimestamp()), // Initialize member variable
            screenManager(manager) {           // Initialize member variable
            this->pName = this->screenName; // Assuming pName is a member of CreatedScreen
        }
        
        void displayHeader() override;
        void displayProcessSmi() override;
};

#endif