#ifndef CREATEDSCREEN_H
#define CREATEDSCREEN_H

#include <string>
#include "Screen.h"
#include "ScreenManager.h"


using namespace std;

class CreatedScreen : public Screen {

    private:
        string screenName;
        string creationTimestamp;
        ScreenManager* screenManager;  // Pointer to ScreenManager for switching screens

    public:
        CreatedScreen(const string& name, ScreenManager* manager);  // Pass ScreenManager in constructor
        
        void display() override;
        void displayHeader();
        void handleCommand(const string& command) override;
        
        const string& getScreenName() const;
        string getTimestamp() const;
};

#endif