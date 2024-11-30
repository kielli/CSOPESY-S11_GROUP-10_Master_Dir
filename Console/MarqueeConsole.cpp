#include "MarqueeConsole.h"

MarqueeConsole::MarqueeConsole() : AConsole("MarqueeConsole")
{
}

void MarqueeConsole::onEnabled()
{
	this->displayHeader();
}

void MarqueeConsole::display()
{
}

void MarqueeConsole::process()
{
	this->PollKeyboard(KEH);
}

int MarqueeConsole::getConsoleHeight() const
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(ConsoleManager::getInstance()->getConsoleHandle(), &csbi);
	return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

int MarqueeConsole::getConsoleWidth() const
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(ConsoleManager::getInstance()->getConsoleHandle(), &csbi);
	return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void MarqueeConsole::displayHeader() const
{
	std::cout << "*****************************************" << std::endl;
	std::cout << "* Displaying a Marquee Console!         *" << std::endl;
	std::cout << "*****************************************";
}

void MarqueeConsole::printMarqueeText() const
{
	ConsoleManager::getInstance()->setCursorPosition(coordX, coordY);
	std::cout << std::setfill(' ');
	std::cout << this->marqueeText;
}

void MarqueeConsole::updateMarqueeCOORDs()
{
	this->coordX += this->borderX ? -1 : 1;
	this->coordY += this->borderY ? -1 : 1;

	// Horizontal bounds (adjust as needed)
	if (this->coordX >= this->length - this->marqueeText.size()) {
		this->borderX = true;
	}
	else if (this->coordX < 1) {
		this->borderX = false;
	}

	// Vertical bounds (stop before the reserved rows)
	int reservedHeight = 3; // 2 rows for input/confirmation + 1 extra buffer
	if (this->coordY >= this->width - reservedHeight) {
		this->borderY = true;
	}
	else if (this->coordY < 3) { // Top boundary
		this->borderY = false;
	}
}

void MarqueeConsole::printEnterCommandFlag() const
{
	// Reserve the second-to-last row for the command flag
	ConsoleManager::getInstance()->setCursorPosition(0, this->getConsoleHeight() - 2);
	std::cout << this->enterCommandFlag << std::flush;
}

void MarqueeConsole::printKeyDownUpInput(const KeyboardEventHandler& KEH) const
{
	// Position after the enterCommandFlag in the reserved row
	ConsoleManager::getInstance()->setCursorPosition(this->enterCommandFlag.size(), this->getConsoleHeight() - 2);

	// Print the current input immediately
	std::cout << KEH.getUserInput() << std::flush;
}

void MarqueeConsole::printCommandConfirm() const
{
	if (!KEH.getUserInput().empty()) {
		// Use the last row for confirmation
		ConsoleManager::getInstance()->setCursorPosition(0, this->getConsoleHeight() - 1);
		std::cout << "Command Processed in MARQUEE_CONSOLE: " << KEH.getUserInput() << std::flush;
	}

}

void MarqueeConsole::updateMarqueeConsole()
{
	// Clear only the marquee area (excluding the reserved rows)
	for (int y = 3; y < this->width - 3; ++y) {
		ConsoleManager::getInstance()->setCursorPosition(0, y);
		std::cout << std::string(this->length, ' ') << std::flush;
	}

	system("cls");
	this->displayHeader();
	this->printMarqueeText();
	this->updateMarqueeCOORDs();
	this->printEnterCommandFlag();
	this->printCommandConfirm();
}

void MarqueeConsole::PollKeyboard(KeyboardEventHandler& KEH)
{
	int i = 0;
	isMarqueeRunning = true;

	while (isMarqueeRunning) {
		if (i++ == refreshRate) {
			this->updateMarqueeConsole();
			i = 0;
		}

		if (_kbhit()) {
			char key = _getch();
			if (GetAsyncKeyState(key) & 0x8000) {
				KEH.onKeyDown(key);
			}
			else {
				KEH.onKeyUp(key);
			}

			// Immediately print the key press
			printKeyDownUpInput(KEH);
		}

		if (KEH.getUserInput() == "exit") {
			isMarqueeRunning = false;
			system("cls");
			ConsoleManager::getInstance()->returnToPreviousConsole();
		}
	}
}
