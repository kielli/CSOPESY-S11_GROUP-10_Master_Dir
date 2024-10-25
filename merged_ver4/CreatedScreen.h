#ifndef CREATEDSCREEN_H
#define CREATEDSCREEN_H

#include "ScreenManager.h"
#include "Screen.h"

#include <string>

using namespace std;

class ScreenManager; 

class CreatedScreen : public Screen {
    private:
        string screenName;
        string creationTimestamp;
        
        ScreenManager* screenManager;  // Pointer to ScreenManager for switching screens

    public:
        CreatedScreen(const string& name, ScreenManager* manager);  // Pass ScreenManager in constructor
        
        void displayScreen() override;
        void handleCommand(const string& command) override;
        
        void displayCreatedScreenHeader();
        
        const string& getScreenName() const;
};

#endif