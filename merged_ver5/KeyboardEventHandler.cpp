#include "KeyboardEventHandler.h"

#include <conio.h>
#include <windows.h>
#include <iostream>

void KeyboardEventHandler::OnKeyDown(char key) {
    if (GetAsyncKeyState(VK_BACK) & 0x8000 && !keyInputs.empty()) { // Backspace key
        keyInputs.pop_back();
    }
    else if (GetAsyncKeyState(VK_RETURN) & 0x8000) { // Enter key
        userInput = storeInput;
        storeInput.clear();
        keyInputs.clear();
    }
    else if (GetAsyncKeyState(VK_SPACE) & 0x8000) { // Space key
        keyInputs.push_back(' ');
    }
    else if (GetAsyncKeyState(VK_SHIFT) & 0x8000 || GetAsyncKeyState(VK_CAPITAL) & 0x8000) { // Shift or Caps Lock
        keyInputs.push_back(key);
    }
    storeInput = vectorToString(keyInputs);
}

void KeyboardEventHandler::OnKeyUp(char key) {
    keyInputs.push_back(key);
    storeInput = vectorToString(keyInputs);
}

string KeyboardEventHandler::getUserInput() {
    return this->userInput;
}

std::string KeyboardEventHandler::vectorToString(const std::vector<char>& input) {
    return std::string(input.begin(), input.end());
}