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

    void setCursorPosition(int x, int y) { //Sets the console cursor to a given row (x) and column (y)
        COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void updateMarqueeCoords() { //Updates the coords of the marquee by checking if it has approached the border of the screen, adjusts the direction if screen is approached
        coordX += borderX ? -1 : 1;
        coordY += borderY ? -1 : 1;
        if (coordX >= length - marqueeText.size() - 10) borderX = true;
        else if (coordX < 1) borderX = false;
        if (coordY >= width - 1) borderY = true;
        else if (coordY < 3) borderY = false;
    }

    void printHeader() { //Prints the header of the marquee console
        cout << headerText; 
    }
    void printMarquee() { //Prints the actual marquee text
        setCursorPosition(coordX, coordY); cout << setfill(' ') << marqueeText; 
    }
    void printInputField() { //Prints the input field
        setCursorPosition(0, width+1); cout << inputFieldText; 
    }
    void printStoredInput() { //Prints the character typed by the user by means of keyboard polling
        cout << storeInput; setCursorPosition(inputFieldText.size() + keyInputs.size(), width+1); 
    }
    void printCommandConfirm() { //Prints confirmation of command previously entered
        if (!userInput.empty()) cout << "\nCommand Processed in MARQUEE_CONSOLE: " << userInput; setCursorPosition(inputFieldText.size() + keyInputs.size(), width+1); 
    }

public:
    void updateConsole() { //Step-by-step process of the marquee in order to show different frames
        system("cls");
        printHeader();
        printMarquee();
        updateMarqueeCoords();
        printInputField();
        printStoredInput();
        printCommandConfirm();
    }
};

class IKeyboardEvent { //Acts as the interface for Event Handler
public:
    virtual void OnKeyDown(char key) = 0;
    virtual void OnKeyUp(char key) = 0;
};

class KeyboardEventHandler : public IKeyboardEvent { //Triggers certain actions depending on the button pressed
public:
    void OnKeyDown(char key) override { //Actions done when the keyboard button is pressed down
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
        storeInput = vectorToString(keyInputs); //Temporarily stores the string value stored in the keyInputs vector
    }
    void OnKeyUp(char key) override { //Actions done when the keyboard button has been let go
        keyInputs.push_back(key);
        storeInput = vectorToString(keyInputs);
    }
};

void PollKeyboard(IKeyboardEvent& keyboardEvent, MarqueeConsole& mconsole) { //Manages keyboard polling
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