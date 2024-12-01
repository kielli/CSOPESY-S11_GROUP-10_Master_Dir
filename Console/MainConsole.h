#pragma once
#include <iostream>
#include <Windows.h>
#include <algorithm>
#include <conio.h>

#include "AConsole.h"
#include "../TypeDefRepo.h"
#include "ConsoleManager.h"
#include "../Process/Process.h"
#include "../Layout/DummyProcessLayout.h"
#include "../Scheduler/Scheduler.h"

class MainConsole : public AConsole
{
	public:
		MainConsole();
		void onEnabled() override;
		void display() override;
		void process() override;

	private:
		bool isInitialized = false;
		bool isRunning = true;
		//bool isStopSchedulerTest = false;
		std::atomic<bool> isStopSchedulerTest = false;

		void ASCIITextHeader() const;
		void displayDevelopers() const;

		bool isInitialCommand(String command) const;		// Check if it is initialize command or exit command
		bool validateCommand(String command) const;			// Check if the command is valid
		void commandRecognized(String command) const;		// Display message that the command is recognized
		bool validateScreenCommand(String command) const;	// Check if the command is a screen command

		void executeScreenSwitchCommand(String command) const;
		void executeScreenRedrawCommand(String command) const;
		void executeScreenListCommand();
		void executeSchedulerTestCommand();
		void executeSchedulerStopCommand();
		void executeReportUtilizationCommand() const;

		void executeDummyLayoutCommand() const;
		void executeMarqueeConsoleCommand() const;

		void displayMainProcessSmi();
		void displayVmstat();
};

	
