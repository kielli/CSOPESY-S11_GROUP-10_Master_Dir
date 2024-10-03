#include <iostream>
#include <windows.h>
#include <iomanip>
#include <conio.h>

using namespace std;

class MarqueeConsole{
    int coordX, coordY, length, width;
    string marqueeText = "Hello World in marquee!";
    const char *headerText =
        "*********************************\n"
        "* Displaying a marquee console! *\n"
        "*********************************";
    string inputFieldText = "Enter a command for MARQUEE_COMMAND: ";
    bool borderX, borderY;

    public:
        MarqueeConsole() {
            borderX = false;
            borderY = false;
            coordX = 0;
            coordY = 2;
            length = 120;
            width = 30; 
        }

        void updateConsole() {
            system("cls");
            printHeader();
            printMarquee();
            updateMarqueeCoords();
            checkMarqueeCoords();
            printInputField();
        }
    
    private:
        void setCursorPosition(int x, int y) {
            COORD coord;
            coord.X = x;
            coord.Y = y;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        }

        void updateMarqueeCoords() {
            if(borderX)
                coordX--;
            else
                coordX++;
            if(borderY)
                coordY--;
            else
                coordY++;
        }

        void checkMarqueeCoords() {
            if(coordX > length-marqueeText.size()){
                coordX = length-marqueeText.size();
                borderX = true;
            }
            else if(coordX < 2){
                coordX = 1;
                borderX = false;
            }
            if(coordY > width-2){
                coordY = width-1;
                borderY = true;
            }
            else if(coordY < 2){
                coordY = 2;
                borderY = false;
            }
        }

        void printHeader() {
            cout << headerText << endl;
        }

        void printInputField() {
            setCursorPosition(0,31);
            cout << inputFieldText << endl;
        }

        void printMarquee() {
            setCursorPosition(coordX,coordY);
            cout << setfill(' ') << marqueeText; 
        }

};

class IKeyboardEvent {
    public:
        virtual void OnKeyDown(char key) = 0;
        virtual void OnKeyUp(char key) =0;
};

class KeyboardEventHandler : public IKeyboardEvent {
    public:
        void OnKeyDown(char key) override {

        }
        void OnKeyUp(char key) override {

        }
};

void PollKeyboard(IKeyboardEvent& keyboardEvent) {
    while (true) {
        if(_kbhit()) {
            char key = _getch();

            if (GetAsyncKeyState(key) & 0x8000) {
                keyboardEvent.OnKeyDown(key);
            } else {
                keyboardEvent.OnKeyUp(key);
            }
        }
    }
}

int main(){
    MarqueeConsole mconsole = MarqueeConsole();
    while(true){
        mconsole.updateConsole();
    }

}

