#include "Screen.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

string Screen::getCurrentTime() {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm localTime;
    localtime_s(&localTime, &currentTime);

    stringstream ss;
    ss << put_time(&localTime, "%m/%d/%Y, %I:%M:%S %p");
    return ss.str();
}

Screen::Screen(const std::string& name) : name(name), currentLine(0), totalLines(100), running(true) {
    timestamp = getCurrentTime();
}

Screen::Screen() : name("Unnamed"), currentLine(0), totalLines(100), running(true) {
    timestamp = getCurrentTime();
}

void Screen::displayScreen() const {
    system("cls"); // Clear the screen
    cout << "Screen session: " << name << endl;
    cout << "Current Line: " << currentLine << " / " << totalLines << endl;
    cout << "Screen Created At: " << timestamp << endl;
    cout << "\n" << endl;
    cout << "Type 'exit' to return to the main menu.\n" << endl;
    for (const auto& content : contents) {
        cout << content << endl;
    }
}

void Screen::updateLine() {
    if (currentLine < totalLines) {
        currentLine++;
    }
}

void Screen::addContent(const std::string& content) {
    if (currentLine < totalLines) {
        contents.push_back(content); // Add the new content to the screen's contents
        currentLine++; // Increment the line count
    }
}

void Screen::clearContent() {
    contents.clear(); // Clear the content of the screen
}

void Screen::stop() {
    running = false; // Mark the screen as not running
}

bool Screen::isRunning() const {
    return running;
}

void Screen::start() {
    running = true; // Mark the screen as running
}

std::string Screen::getTimestamp() const {
    return timestamp;
}
