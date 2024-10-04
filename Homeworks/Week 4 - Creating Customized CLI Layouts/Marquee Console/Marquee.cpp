#include <iostream>
#include <windows.h>
#include <iomanip>
#include <conio.h>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std;

// Constant Variables for storing inputs as well as displays
vector<char> keyInputs;
string storeInput, userInput;
const int refreshRate = 100; // Change refresh rate as necessary, higher refresh rate gives slower marquee
const string marqueeText = "Hello World in marquee!";
const char* headerText =
"*********************************\n"
"* Displaying a marquee console! *\n"
"*********************************";
const string inputFieldText = "Enter a command for MARQUEE_CONSOLE: ";

// Function for converting vector<char> to string
string vectorToString(const vector<char>& a) {
    return string(a.begin(), a.end());
}

// Function to get console screen height
int getConsoleHeight() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

// Class for Marquee Manager, works by displaying different frames per rotation in while loop
class MarqueeConsole {
    int coordX = 0, coordY = 2, length = 150, width = 30; // Change length and width as necessary
    bool borderX = false, borderY = false;

    void setCursorPosition(int x, int y) { // Sets the console cursor to a given row (x) and column (y)
        COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void updateMarqueeCoords() { // Updates the coords of the marquee by checking if it has approached the border of the screen, adjusts the direction if screen is approached
        coordX += borderX ? -1 : 1;
        coordY += borderY ? -1 : 1;
        if (coordX >= length - marqueeText.size()) borderX = true;
        else if (coordX < 1) borderX = false;
        if (coordY >= width - 1) borderY = true;
        else if (coordY < 3) borderY = false;
    }

    void printHeader() { // Prints the header of the marquee console
        cout << headerText;
    }
    void printMarquee() { // Prints the actual marquee text
        setCursorPosition(coordX, coordY); cout << setfill(' ') << marqueeText;
    }
    void printInputField() { // Prints the input field at the bottom of the console
        int consoleHeight = getConsoleHeight();
        setCursorPosition(0, consoleHeight - 3); // Adjusted to be 3 lines from the bottom
        cout << inputFieldText;
    }
    void printStoredInput() { // Prints the character typed by the user by means of keyboard polling
        cout << storeInput;
        int consoleHeight = getConsoleHeight();
        setCursorPosition(inputFieldText.size() + keyInputs.size(), consoleHeight - 3);
    }
    void printCommandConfirm() { // Prints confirmation of command previously entered
        if (!userInput.empty()) {
            int consoleHeight = getConsoleHeight();
            setCursorPosition(0, consoleHeight - 2); // Set confirmation message just above the input field
            cout << "\nCommand Processed in MARQUEE_CONSOLE: " << userInput;
        }
    }

public:
    void updateConsole() { // Step-by-step process of the marquee in order to show different frames
        system("cls");
        printHeader();
        printMarquee();
        updateMarqueeCoords();
        printInputField();
        printStoredInput();
        printCommandConfirm();
    }
};

class IKeyboardEvent { // Acts as the interface for Event Handler
public:
    virtual void OnKeyDown(char key) = 0;
    virtual void OnKeyUp(char key) = 0;
};

// Helper function to check if Caps Lock is ON
bool isCapsLockActive() {
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

class KeyboardEventHandler : public IKeyboardEvent { // Triggers certain actions depending on the button pressed
public:
    void OnKeyDown(char key) override { // Actions done when the keyboard button is pressed down
        if (GetAsyncKeyState(VK_BACK) & 0x8000 && !keyInputs.empty()) { // Manages Backspace button event
            keyInputs.pop_back();
        }
        else if (GetAsyncKeyState(VK_RETURN) & 0x8000) { // Manages Enter button event
            userInput = storeInput;
            storeInput.clear();
            keyInputs.clear();
        }
        else if (GetAsyncKeyState(VK_SPACE) & 0x8000) { // Manages Space button event
            keyInputs.push_back(' ');
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
        storeInput = vectorToString(keyInputs); // Temporarily stores the string value stored in the keyInputs vector
    }

    void OnKeyUp(char key) override { // Actions done when the keyboard button has been let go
        // You may not need any specific handling for key up in this case
        keyInputs.push_back(key);
        storeInput = vectorToString(keyInputs);
    }
};

void PollKeyboard(IKeyboardEvent& keyboardEvent, MarqueeConsole& mconsole, atomic<bool>& running) { // Manages keyboard polling
    while (running) {
        if (_kbhit()) { // Basically says 'if keyboard button is pressed'
            char key = _getch(); // Gets the character from the button pressed
            (GetAsyncKeyState(key) & 0x8000 ? keyboardEvent.OnKeyDown(key) : keyboardEvent.OnKeyUp(key)); // Handles Down and Up key states
        }
        if (userInput == "exit") running = false; // If user inputted exit, stop gracefully
        this_thread::sleep_for(chrono::milliseconds(100)); // Add small delay to avoid excessive CPU usage
    }
}

void UpdateMarquee(MarqueeConsole& mconsole, atomic<bool>& running) { // Marquee update loop
    while (running) {
        mconsole.updateConsole();
        this_thread::sleep_for(chrono::milliseconds(refreshRate)); // Set refresh rate for marquee update
    }
}

int main() {
    MarqueeConsole MC;
    KeyboardEventHandler KEH;
    atomic<bool> running(true); // Flag to manage program termination

    // Start threads
    thread keyboardThread(PollKeyboard, ref(KEH), ref(MC), ref(running));
    thread marqueeThread(UpdateMarquee, ref(MC), ref(running));

    // Wait for threads to finish
    keyboardThread.join();
    marqueeThread.join();

    return 0;
}