#ifndef CREATEDSCREEN_H
#define CREATEDSCREEN_H

#include "Screen.h"
#include "ScreenManager.h"
#include <string>

class CreatedScreen : public Screen {

private:
    std::string screenName;
    std::string creationTimestamp;
    ScreenManager* screenManager;  // Pointer to ScreenManager for switching screens

public:
    CreatedScreen(const std::string& name, ScreenManager* manager);  // Pass ScreenManager in constructor
    void display() override;
    void displayHeader();
    void handleCommand(const std::string& command) override;
    const std::string& getScreenName() const;
    std::string getTimestamp() const;
};

#endif
