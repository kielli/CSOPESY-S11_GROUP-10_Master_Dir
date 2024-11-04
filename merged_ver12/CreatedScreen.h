#ifndef CREATEDSCREEN_H
#define CREATEDSCREEN_H

#include "Screen.h"
#include "ScreenManager.h"

#include <string>

using namespace std;

class CreatedScreen : public Screen {
    private:
        string screenName;
        string creationTimestamp;
        int pid;
        ScreenManager* screenManager;  // Pointer to ScreenManager for switching screens
        bool finished = false;

    public:
       /* CreatedScreen(const string& name, ScreenManager* manager, int numInstruction);*/
        /*CreatedScreen(const string& name, ScreenManager* manager, int pId, int numInstruction);*/
    
        CreatedScreen(const string& name, ScreenManager* manager, int PID, int numInstruction)
            : Screen(name, PID, numInstruction), // Base class constructor
            screenName(name),                  // Initialize member variable
            pid(PID),                          // Initialize member variable
            creationTimestamp(this->getTimestamp()), // Initialize member variable
            screenManager(manager) {           // Initialize member variable
            this->pName = this->screenName; // Assuming pName is a member of CreatedScreen
        }
        void display() override;
        void displayHeader();
        void handleCommand(const string& command) override;
        void displayProcessSmi();

        const string& getScreenName() const;
};

#endif