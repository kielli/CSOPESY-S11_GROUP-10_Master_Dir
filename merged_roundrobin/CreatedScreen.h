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
        ScreenManager* screenManager;  // Pointer to ScreenManager for switching screens
        bool finished = false;

    public:
        CreatedScreen(const std::string& name, ScreenManager* manager, int numInstruction);
        CreatedScreen(const std::string& name, ScreenManager* manager, int pId, int numInstruction): Screen(name, pId, numInstruction){}
    
        void display() override;
        void displayHeader();
        void handleCommand(const string& command) override;
        void displayProcessSmi();

        const string& getScreenName() const;
};

#endif