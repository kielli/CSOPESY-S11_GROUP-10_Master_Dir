
#include <iostream>
//#include <WinUser.h>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>
#include "KeyboardEventHandler.h"
#include "MarqueeConsole.h"

#include "Screen.h"

using namespace std;

void Screen::CreateMarquee() {
    
    MarqueeConsole MC;
    KeyboardEventHandler KEH;
    this->userInput = KEH.userInput;
    this->keyInputs = KEH.keyInputs;
    this->storeInput = KEH.storeInput;
   
    atomic<bool> running(true); // Flag to manage program termination

  
  

    // Start threads
    thread keyboardThread(PollKeyboard, ref(KEH), ref(MC), ref(running));
    thread marqueeThread(UpdateMarquee, ref(MC), ref(running));

    // Wait for threads to finish
    keyboardThread.join();
    marqueeThread.join();
}



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

void Screen::PollKeyboard(IKeyboardEvent& keyboardEvent, MarqueeConsole& mconsole, atomic<bool>& running){
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

//static void PollKeyboard(KeyboardEventHandler& keyboardEvent, MarqueeConsole& mconsole, atomic<bool>& running) {
//    while (running) {
//        if (_kbhit()) { // Basically says 'if keyboard button is pressed'
//            char key = _getch(); // Gets the character from the button pressed
//            (GetAsyncKeyState(key) & 0x8000 ? keyboardEvent.OnKeyDown(key) : keyboardEvent.OnKeyUp(key)); // Handles Down and Up key states
//        }
//
//        if (keyboardEvent.storeInput == "exit") running = false; // If user inputted exit, stop gracefully
//        this_thread::sleep_for(chrono::milliseconds(100)); // Add small delay to avoid excessive CPU usage
//    }
//}