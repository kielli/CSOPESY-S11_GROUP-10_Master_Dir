#include <iostream>
#include <windows.h>
#include <vector>
#include <conio.h>
#include <atomic>
#include <thread>

#include "KeyboardEventHandler.h"

using namespace std;

// Handles key down events
void KeyboardEventHandler::OnKeyDown(char key) {
    if (GetAsyncKeyState(VK_BACK) & 0x8000 && !this->keyInputs.empty()) { // Backspace event
        this->keyInputs.pop_back();
    }
    else if (GetAsyncKeyState(VK_RETURN) & 0x8000) { // Enter event
        this->userInput = this->storeInput;
        this->storeInput.clear();
        this->keyInputs.clear();
    }
    else if (GetAsyncKeyState(VK_SPACE) & 0x8000) { // Space event
        this->keyInputs.push_back(' ');
    }
    else { // Normal key press handling
        bool shiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
        bool capsLock = isCapsLockActive();

        if (isalpha(key)) { // Check if the key is an alphabet character
            if ((shiftPressed && !capsLock) || (!shiftPressed && capsLock)) {
                key = toupper(key); // Make uppercase
            }
            else {
                key = tolower(key); // Make lowercase
            }
        }
        keyInputs.push_back(key);
    }
    this->storeInput = vectorToString(this->keyInputs);
}

// Handles key up events
void KeyboardEventHandler::OnKeyUp(char key) {
    keyInputs.push_back(key);
    storeInput = vectorToString(keyInputs);
}

// Checks if Caps Lock is active
bool KeyboardEventHandler::isCapsLockActive() {
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

// Converts vector<char> to string
std::string KeyboardEventHandler::vectorToString(const std::vector<char>& a) {
    return std::string(a.begin(), a.end());
}

