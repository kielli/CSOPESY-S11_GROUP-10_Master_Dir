#include <iostream>
#include <windows.h>
#include <iomanip>
#include <conio.h>
#include <vector>

using namespace std;

//Constant Variables for storing inputs as well as displays
vector<char> keyInputs;
string storeInput, userInput;

const int refreshRate = 2000; //Change refresh rate as necessary, higher refresh rate gives slower marquee

const string marqueeText = "Hello World in marquee!";
const char *headerText =
                        "*********************************\n"
                        "* Displaying a marquee console! *\n"
                        "*********************************";

const string inputFieldText = "Enter a command for MARQUEE_CONSOLE: ";

//Function for converting vector<char> to string
string vectorToString(const vector<char>& a) {
    return string(a.begin(), a.end());
}

//Class for Marquee Manager, Works by displaying different frames per rotation in while loop
class MarqueeConsole {
    int coordX = 0, coordY = 2, length = 150, width = 30; //Change length and width as necessary
    bool borderX = false, borderY = false;

    //Sets the console cursor to a given row (x) and column (y)
    void setCursorPosition(int x, int y) {
        COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    //Updates the coords of the marquee by checking if it has approached the border of the screen, adjusts the direction if screen is approached
    void updateMarqueeCoords() {
        coordX += borderX ? -1 : 1;
        coordY += borderY ? -1 : 1;

        if (coordX >= length - marqueeText.size()) 
            borderX = true;
        else if (coordX < 1) 
            borderX = false;
        if (coordY >= width - 1) 
            borderY = true;
        else if (coordY < 3) 
            borderY = false;
    }

    //Prints the header of the marquee console
    void printHeader() {
        cout << headerText; 
    }

    //Prints the actual marquee text
    void printMarquee() { 
        setCursorPosition(coordX, coordY); cout << setfill(' ') << marqueeText; 
    }

    //Prints the input field
    void printInputField() {
        setCursorPosition(0, width+1); cout << inputFieldText; 
    }

    //Prints the character typed by the user by means of keyboard polling
    void printStoredInput() {
        cout << storeInput; setCursorPosition(inputFieldText.size() + keyInputs.size(), width+1); 
    }

    //Prints confirmation of command previously entered
    void printCommandConfirm() {
        if (!userInput.empty()) cout << "\nCommand Processed in MARQUEE_CONSOLE: " << userInput; setCursorPosition(inputFieldText.size() + keyInputs.size(), width+1); 
    }

public:
    //Step-by-step process of the marquee in order to show different frames
    void updateConsole() {
        system("cls");
        printHeader();
        printMarquee();
        updateMarqueeCoords();
        printInputField();
        printStoredInput();
        printCommandConfirm();
    }
};

// Helper function to check if Caps Lock is ON
bool isCapsLockActive() {
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

//Acts as the interface for Event Handler
class IKeyboardEvent { 
public:
    virtual void OnKeyDown(char key) = 0;
    virtual void OnKeyUp(char key) = 0;
};

//Triggers certain actions depending on the button pressed
class KeyboardEventHandler : public IKeyboardEvent {
public:

    //Actions done when the keyboard button is pressed down
    void OnKeyDown(char key) override {
        if (GetAsyncKeyState(VK_BACK) & 0x8000 && !keyInputs.empty()) { //Manages Backspace button event
            keyInputs.pop_back();
        } else if (GetAsyncKeyState(VK_RETURN) & 0x8000) { //Manages Enter button event
            userInput = storeInput;
            storeInput.clear();
            keyInputs.clear();
        } else if (GetAsyncKeyState(VK_SPACE) & 0x8000) { //Manages Space button event
            keyInputs.push_back(' ');
        } else if (GetAsyncKeyState(VK_SHIFT) & 0x8000 || GetAsyncKeyState(VK_CAPITAL) & 0x8000) { //Manages Shift button event
            keyInputs.push_back(key);
        }
         else { // Handle normal key press, with capitalization checks
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
        storeInput = vectorToString(keyInputs); //Temporarily stores the string value stored in the keyInputs vector
    }
    void OnKeyUp(char key) override { //Actions done when the keyboard button has been let go
        keyInputs.push_back(key);
        storeInput = vectorToString(keyInputs);
    }
};

//Manages keyboard polling
void PollKeyboard(IKeyboardEvent& keyboardEvent, MarqueeConsole& mconsole) {
    int i = 0;
    while (true) {
        if (i++ == refreshRate) { //Manages the refresh rate, frame will only update when the value of i reaches the refresh rate value
            mconsole.updateConsole();
            i = 0;
        }
        if (_kbhit()) { //Basically says 'if keyboard button is pressed'
            char key = _getch(); //Gets the character from the button pressed
            (GetAsyncKeyState(key) & 0x8000 ? keyboardEvent.OnKeyDown(key) : keyboardEvent.OnKeyUp(key)); //Handles Down and Up key states
        }
        if (userInput == "exit") exit(0); //If user inputted exit, program closes
    }
}

int main() {
    MarqueeConsole MC;
    KeyboardEventHandler KEH;
    PollKeyboard(KEH, MC);
}