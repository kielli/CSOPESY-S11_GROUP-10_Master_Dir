#include <iostream>
#include <windows.h>
//#include <WinUser.h>
#include <iomanip>
#include <chrono>
#include <thread>

#include "MarqueeConsole.h"

// Updates the console display
void MarqueeConsole::updateConsole() {
    system("cls");
    printHeader();
    printMarquee();
    updateMarqueeCoords();
    printInputField();
    printStoredInput();
    printCommandConfirm();
}

// Updates the coordinates of the marquee
void MarqueeConsole::updateMarqueeCoords() {
    coordX += borderX ? -1 : 1;
    coordY += borderY ? -1 : 1;

    if (coordX >= static_cast<int>(length) - static_cast<int>(marqueeText.size()))
        borderX = true;
    else if (coordX < 1)
        borderX = false;
    if (coordY >= width - 1)
        borderY = true;
    else if (coordY < 3)
        borderY = false;
}

// Prints the header
void MarqueeConsole::printHeader() {
    std::cout << this->headerText << std::endl;
}

// Prints the marquee text
void MarqueeConsole::printMarquee() {
    setCursorPosition(coordX, coordY);

    cout << std::setfill(' ') << this->marqueeText;
}

// Prints the input field
void MarqueeConsole::printInputField() {
    int consoleHeight = this->getConsoleHeight();

    setCursorPosition(0, consoleHeight - 3); // Adjusted to be 3 lines from the bottom
    cout << this->inputFieldText;
}

// Prints the stored user input
void MarqueeConsole::printStoredInput() {
    cout << this->keyboardManager.storeInput;

    int consoleHeight = this->getConsoleHeight();
    setCursorPosition(inputFieldText.size() + this->keyboardManager.keyInputs.size(), consoleHeight - 3);
}

// Prints command confirmation message
void MarqueeConsole::printCommandConfirm() {
    if (!this->keyboardManager.userInput.empty()) {
        int consoleHeight = this->getConsoleHeight();
        setCursorPosition(0, consoleHeight - 2); // Set confirmation message just above the input field
        cout << "\nCommand Processed in MARQUEE_CONSOLE: " << this->keyboardManager.userInput;
    }
}

// Gets the console screen height
int MarqueeConsole::getConsoleHeight() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

// Sets the console cursor to a given row (x) and column (y)
void MarqueeConsole::setCursorPosition(int x, int y) {
    COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
