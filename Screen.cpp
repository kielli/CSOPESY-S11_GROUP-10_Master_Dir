
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>
//#include <WinUser.h>

#include "Screen.h"

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

int Screen::getTotalLines() const {
    return this->totalLines;
}

void Screen::print_process(const string& command) {
    this->printAndStore(command + "command recognized. Doing something.");
}

void Screen::print_error(const string& command) {
    this->printAndStore("Unknown command: " + command);
}

void Screen::createMarquee() {
    marqueeRunning = true;

    // Start thread
    keyboardThread = std::thread([this]() {
        this->PollKeyboard(keyboardEvent, marqueeConsole, marqueeRunning);
        });

    marqueeThread = std::thread([this]() {
        this->UpdateMarquee(marqueeConsole, marqueeRunning);
        });
}

// Wait for thread to finish
void Screen::stopMarquee() {
    marqueeRunning = false;

    if (keyboardThread.joinable()) {
        keyboardThread.join();
    }
    if (marqueeThread.joinable()) {
        marqueeThread.join();
    }
}

void Screen::PollKeyboard(IKeyboardEvent& keyboardEvent, MarqueeConsole& mconsole, atomic<bool>& running) {
    while (running) {
        if (_kbhit()) { // Basically says 'if keyboard button is pressed'
            char key = _getch(); // Gets the character from the button pressed
            (GetAsyncKeyState(key) & 0x8000 ? keyboardEvent.OnKeyDown(key) : keyboardEvent.OnKeyUp(key)); // Handles Down and Up key states
        }

        if (this->userInput == "exit") running = false; // If user inputted exit, stop gracefully
        this_thread::sleep_for(chrono::milliseconds(100)); // Add small delay to avoid excessive CPU usage
    }
}

void Screen::UpdateMarquee(MarqueeConsole& mconsole, atomic<bool>& running) {
    while (running) {
        mconsole.updateConsole();
        this_thread::sleep_for(chrono::milliseconds(this->refreshRate)); // Set refresh rate for marquee update
    }
}