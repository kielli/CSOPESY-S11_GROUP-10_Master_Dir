#pragma once
#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <conio.h>
#include <vector>

#include "AConsole.h"
#include "../TypeDefRepo.h"
#include "../Event/KeyboardEventHandler.h"
#include "ConsoleManager.h"




class MarqueeConsole : public AConsole
{
public:
	MarqueeConsole();
	void onEnabled() override;
	void display() override;
	void process() override;

private:
	bool isMarqueeRunning = true;
	String marqueeText = "Hello World in marquee!";
	String enterCommandFlag = "Enter a command for MARQUEE_CONSOLE: ";

	int coordX = 0;
	int coordY = 0;
	int length = 120;
	int width = 30;
	bool borderX = false;
	bool borderY = false;
	int refreshRate = 2000;
	
	KeyboardEventHandler KEH;

	int getConsoleHeight() const;
	int getConsoleWidth() const;

	void displayHeader() const;
	void printMarqueeText() const;
	void updateMarqueeCOORDs();
	void printEnterCommandFlag() const;
	void printKeyDownUpInput(const KeyboardEventHandler& KEH) const;
	void printCommandConfirm() const;

	void updateMarqueeConsole();
	void PollKeyboard(KeyboardEventHandler& KEH);
};

