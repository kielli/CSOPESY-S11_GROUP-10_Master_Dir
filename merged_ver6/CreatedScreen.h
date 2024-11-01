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
    //CreatedScreen(const string& name, ScreenManager* manager);  // Pass ScreenManager in constructor

    CreatedScreen(const std::string& name, ScreenManager* manager, int numInstruction);

    void display() override;
    void displayHeader();
    void handleCommand(const string& command) override;

    const string& getScreenName() const;
    string getTimestamp() const;
};

#endif