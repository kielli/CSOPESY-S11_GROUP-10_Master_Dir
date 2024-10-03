#include <iostream>
#include <windows.h>
#include <conio.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <string>

//Keyboard event interface
using namespace std;
atomic<bool> running(true); // Atomic flag to control the running state
mutex mtx; // Mutex for thread safety
class IKeyboardEvent {
public:
	virtual void OnKeyDown(char key) = 0;
	virtual void OnKeyUp(char key) = 0;
};
//Keyboard event implementation
class KeyboardEventHandler : public IKeyboardEvent {
public:
	void OnKeyDown(char key) override {
		std::cout << "Key Down: " << key << std::endl;
	}
	void OnKeyUp(char key) override {
		std::cout << "Key Up: " << key << std::endl;
	}
};
void displayHeader() {
	cout << "****************************************" << endl;
	cout << "*  Displaying a marquee console!   *" << endl;
	cout << "****************************************" << endl;
}
void moveXY(int x, int y) {
	COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void bounceText() {
	const string text = "Hello world in marquee!";

	const int xMinPos = 10; // Minimum X boundary
	const int xMaxPos = 80; // Maximum X boundary

	const int yMinPos = 6;  // Minimum Y boundary
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
//Keyboard Polling
void PollKeyboard(IKeyboardEvent& keyboardEvent) {
	string command;
	const string text = "Hello world in marquee!";

	const int xMinPos = 10; // Minimum X boundary
	const int xMaxPos = 80; // Maximum X boundary

	const int yMinPos = 6;  // Minimum Y boundary
	const int yMaxPos = 15; // Maximum Y boundary

	int currX = xMinPos;                    // Start at minimum X
	int currY = (yMinPos + yMaxPos) / 2;    // Start at vertical center
	int comX = 0;

	int xDirection = 1; // 1 for right, -1 for left
	int yDirection = 1; // 1 for down, -1 for up
	moveXY(0, 17);
	cout << "Enter a command for MARQUEE_CONSOLE: ";
	while (true) {
		if (_kbhit()) {
			
			char key = _getch();

			if (key == '\r'){
			comX = 0;
			moveXY(comX, 18);
			cout << "\x1b[2K";
			cout << "\r";
			}
			else {
				
				moveXY(comX, 18);
				cout << key;
				comX += 1;
			}
		}
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
		
		//Other programs here marquee probably
	}
}

/*void keyboardInterruptHandler(){
	if(_kbhit()){
		char key = _getch();
		std::cout << "Key pressed: " << key << std::endl;
	}
}*/
int main() {
	KeyboardEventHandler keyboardHandler;
	displayHeader();
	PollKeyboard(keyboardHandler);
	return 0;
	/*while(true){
		Sleep(100);
		keyboardInterruptHandler();
	}*/
	return 0;
}