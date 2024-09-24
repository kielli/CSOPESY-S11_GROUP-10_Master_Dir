#ifndef SCREEN_H
#define SCREEN_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <chrono>

class Screen {
public:
    Screen(const std::string& name);
    Screen();

    void displayScreen() const;
    void updateLine();
    void addContent(const std::string& content);
    void clearContent();
    void stop();
    bool isRunning() const;
    void start();
    std::string getTimestamp() const;

private:
    std::string name;
    int currentLine;
    int totalLines;
    std::string timestamp;
    bool running; // Flag to indicate if the screen is active
    std::vector<std::string> contents;
    std::string getCurrentTime();
};

#endif // SCREEN_H
