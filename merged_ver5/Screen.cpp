#include "Screen.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

// Adds a line to contents and updates line counters
void Screen::addLine(const string& line) {
    this->contents.push_back(line);
    this->currentLineNumber++;
    this->totalLines++;
}

void Screen::StoreAll(const vector<string>& lines) {
    for (const auto& line : lines) {
        addLine(line);
    }
}

// Stores and prints a line 
void Screen::printAndStore(const string& line) {
    addLine(line);               
    cout << line << std::endl; 
}

// Stores a line in contents without printing
void Screen::Store(const string& line) {
     addLine(line); // calls addLine to store the line and updates counters
}

// Deletes stored contents
void Screen::deleteContent(const vector<string>& lines) {
    this->contents.clear();
}

// Redraws the screen by printing all stored contents
void Screen::redrawScreen() {
    for (const auto& line : contents) {
        cout << line << endl;
    }
}

// Returns updated line number
int Screen::updateNumberOfLines() const {
    return this->currentLineNumber;
}

// Prints the processed command
void Screen::print_process(const string& command) {
    this->printAndStore(command + "command recognized. Doing something.");
}

// Prints error message
void Screen::print_error(const string& command) {
    this->printAndStore("Unknown command: " + command);
}

// Returns total number of lines
int Screen::getTotalLines() const {
    return this->totalLines;
}

// Returns running status
bool Screen::getRunningStatus() {
    return this->isRunning;
}

// Returns active status
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

// Returns timestamp
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