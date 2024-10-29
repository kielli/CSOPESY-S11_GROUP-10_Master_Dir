#include "Screen.h"

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>
//#include <WinUser.h>

// Adds a line to contents and updates line counters
void Screen::addLine(const string& line) {
    this->contents.push_back(line);
    this->totalLines++;
    this->currentLineNumber++;
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

// Prints the processed command
void Screen::print_process(const string& command) {
    this->printAndStore(command + " command recognized. Doing something...");
}

// Print error message
void Screen::print_error(const string& command) {
    this->printAndStore("Unknown command: " + command + "\n");
}

// Deletes stored contents
void Screen::deleteContent(const vector<string>& lines) {
    this->contents.clear();
}

// Updates line number
int Screen::updateNumberOfLines() const {
    return this->currentLineNumber;
}

// Gets total number of lines
int Screen::getTotalLines() const {
    return this->totalLines;
}

// Redraws the screen by printing all stored contents
void Screen::redrawScreen() {
    for (const auto& line : contents) {
        cout << line << endl;
    }
}

// Get timestamp
string Screen::getTimestamp() const {
    auto now = chrono::system_clock::now(); // getting the current time
    time_t currentTime = chrono::system_clock::to_time_t(now); // converting time to time_t (calendar time)
    tm localTime; //tm structure to hold the local time (year, month, day, hour, etc.)
    stringstream ss; // for formatting output

    // use localtime_s for thread safety
    localtime_s(&localTime, &currentTime); // converting the current time into a local time 

    // %a = weekday, %b = month name, %d = day, %H = hour, %M = minute, %S = second, %Y = year
    ss << put_time(&localTime, "%a %b %d %H:%M:%S %Y \n");

    return ss.str();
}