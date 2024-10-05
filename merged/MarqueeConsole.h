#ifndef MARQUEE_CONSOLE_H
#define MARQUEE_CONSOLE_H

#include <string>
#include "KeyboardEventHandler.h"

using namespace std;

class MarqueeConsole {
public:
    int coordX, coordY;
    bool borderX, borderY;
    int length, width;

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
