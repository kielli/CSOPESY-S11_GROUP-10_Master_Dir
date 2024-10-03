#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>

using namespace std;

atomic<bool> running(true); // Atomic flag to control the running state
mutex mtx; // Mutex for thread safety

// Display the header
void displayHeader() {
    cout << "****************************************" << endl;
    cout << "*  Displaying a marquee console!   *" << endl;
    cout << "****************************************" << endl;
}

// Moves the cursor position
void moveXY( short x, short y ) 
{ 
    // to get control over the console
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE) ; 

    // declare cursor position as coord structure
    COORD position = { x, y } ; 
     
    // function to give control on cursor position
    SetConsoleCursorPosition( hStdout, position ) ; 
}  

void bounceText() {
    const string text = "Hello world in marquee!";
    
    const int xMinPos = 10; // Minimum X boundary
    const int xMaxPos = 80; // Maximum X boundary

    const int yMinPos = 8;  // Minimum Y boundary
    const int yMaxPos = 15; // Maximum Y boundary

    int currX = xMinPos;                    // Start at minimum X
    int currY = (yMinPos + yMaxPos) / 2;    // Start at vertical center

    int xDirection = 1; // 1 for right, -1 for left
    int yDirection = 1; // 1 for down, -1 for up

    while (running) {
        {
            lock_guard<mutex> lock(mtx); // Lock the mutex to safely update and clear the console

            // Clear previous position and text
            moveXY(currX, currY);
            cout << string(text.length(), ' ');

            // Update x and y positions
            currX += xDirection;
            currY += yDirection;

            // If x has collided with one of the boundaries, change horizontal direction
            if (currX >= xMaxPos - static_cast<int>(text.length()) || currX < xMinPos) {
                xDirection *= -1;
            }

            // If y has collided with one of the boundaries, change vertical direction
            if (currY >= yMaxPos || currY < yMinPos) {
                yDirection *= -1; 
            }

            // New position of bouncing text
            moveXY(currX, currY);
            cout << text; // Display the bouncing text
        }

        // For slow movement of the text
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

void inputCommand() {
    string command;

    while (running) {
        {
            lock_guard<mutex> lock(mtx); // Lock the mutex for input area

            // Command prompt position
            moveXY(0, 17); 
            cout << "Enter a command for MARQUEE_CONSOLE: ";
        }

        // Wait for user input
        getline(cin, command); 

        {
            lock_guard<mutex> lock(mtx); // Lock the mutex to print the command

            // Entered prompt fixed position
            moveXY(0, 18);
            cout << "Command processed in MARQUEE_CONSOLE: " << command << endl; // Display the entered command
        }

        // Check if the command is "exit"
        if (command == "exit") {
            running = false; // Exit command
        }
    }
}

int main() {
    displayHeader();

    // Thread for the bouncing text
    thread bounceThread(bounceText);

    // Thread for input command
    thread commandThread(inputCommand);

    // Wait for both threads to finish
    bounceThread.join();
    commandThread.join();

    return 0;
}
