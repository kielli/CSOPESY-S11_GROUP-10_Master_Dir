#include "Screen.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

void Screen::printAndStore(const string& line) {
    this->contents.push_back(line);
    cout << line << std::endl;
    this->totalLines++;
    this->currentLineNumber++;
}

void Screen::StoreAll(const vector<string>& lines) {
    for (const auto& line : lines) {
        this->contents.push_back(line);
        this->currentLineNumber++;
        this->totalLines++;
    }
}

void Screen::deleteContent(const vector<string>& lines) {
    this->contents.clear();
}

void Screen::Store(const string& line) {
    this->contents.push_back(line);
    this->totalLines++;
    this->currentLineNumber++;
}

void Screen::redrawScreen() {
    for (const auto& line : contents) {
        cout << line << endl;
    }
}

int Screen::updateNumberOfLines() const {
    return this->currentLineNumber;
}

string Screen::getTimestamp() const {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm localTime; //tm structure to hold the local time

    // use localtime_s for thread safety
    localtime_s(&localTime, &currentTime);

    stringstream ss;
    ss << put_time(&localTime, "%m/%d/%Y, %I:%M:%S %p");

    return ss.str();
}

int Screen::getTotalLines() const {
    return this->totalLines;
}

void Screen::print_process(const string& command) {
    this->printAndStore(command + "command recognized. Doing something.");
}

void Screen::print_error(const string& command) {
    this->printAndStore("Unknown command: " + command);
}

bool Screen::getRunningStatus() {
    return this->isRunning;
}

bool Screen::getActiveStatus() {
    return this->isActive;
}

void Screen::setRunningStatusOff() {
    this->isRunning = false;
}

void Screen::setActiveStatusOn() {
    this->isActive = true;
}

void Screen::setActiveStatusOff() {
    this->isActive = false;
}