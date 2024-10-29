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

    public:
        CreatedScreen(const string& name, ScreenManager* manager);  // Pass ScreenManager in constructor

        void display() override;
        void handleCommand(const string& command) override;

        void displayHeader(); // Created screen header

        const string& getScreenName() const;
};

#endif