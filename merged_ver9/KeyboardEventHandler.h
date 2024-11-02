#ifndef KEYBOARDEVENTHANDLER_H
#define KEYBOARDEVENTHANDLER_H

#include <iostream>
#include <windows.h>
#include <iomanip>
#include <conio.h>
#include <vector>
#include <string>

using namespace std;

class IKeyboardEvent {
    public:
        virtual void OnKeyDown(char key) = 0;
        virtual void OnKeyUp(char key) = 0;
    };

class KeyboardEventHandler : public IKeyboardEvent {
    public:
        void OnKeyDown(char key) override;
        void OnKeyUp(char key) override;
        string getUserInput();

    private:
        vector<char> keyInputs;
        string userInput;
        string storeInput;
        string vectorToString(const std::vector<char>& input);
};

#endif
