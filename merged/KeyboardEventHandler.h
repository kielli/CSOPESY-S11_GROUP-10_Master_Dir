#ifndef KEYBOARD_EVENT_HANDLER_H
#define KEYBOARD_EVENT_HANDLER_H

#include <vector>
#include <string>
#include <atomic>

using namespace std;

// Acts as the interface for Event Handler
class IKeyboardEvent {
public:
    virtual void OnKeyDown(char key) = 0;
    virtual void OnKeyUp(char key) = 0;
};

class KeyboardEventHandler : public IKeyboardEvent {
public:
    std::vector<char> keyInputs;
    std::string storeInput, userInput;

    void OnKeyDown(char key) override;
    void OnKeyUp(char key) override;
    
    bool isCapsLockActive();
    std::string vectorToString(const std::vector<char>& a);
};

#endif // KEYBOARD_EVENT_HANDLER_H
