#ifndef MARQUEE_CONSOLE_H
#define MARQUEE_CONSOLE_H

#include <string>
#include "KeyboardEventHandler.h"

using namespace std;

class MarqueeConsole {
public:
    int coordX = 0, coordY = 2;
    int length = 150, width = getConsoleHeight()-3; // Change length and width as necessary
    bool borderX = false, borderY = false;

    string headerText =
                            "*********************************\n"
                            "* Displaying a marquee console! *\n"
                            "*********************************";

    string marqueeText = "Hello World in marquee!";
    string inputFieldText = "Enter a command for MARQUEE_CONSOLE: ";

    KeyboardEventHandler keyboardManager;

    void updateConsole();
    void updateMarqueeCoords();
    void printHeader();
    void printMarquee();
    void printInputField();
    void printStoredInput();
    void printCommandConfirm();
    int getConsoleHeight();
    void setCursorPosition(int x, int y);
};

#endif // MARQUEE_CONSOLE_H
