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

int Screen::getTotalLines() const {
    return this->totalLines;
}

void Screen::print_process(const string& command) {
    this->printAndStore(command + "command recognized. Doing something.");
}

void Screen::print_error(const string& command) {
    this->printAndStore("Unknown command: " + command);
}

bool Screen::getActiveStatus() {
    return this->isActive;
}

void Screen::setActiveStatusOn() {
    this->isActive = true;
}

void Screen::setActiveStatusOff() {
    this->isActive = false;
}