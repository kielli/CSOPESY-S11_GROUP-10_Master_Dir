#include "BaseScreen.h"

#include "../Console/ConsoleManager.h"

BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) : AConsole(processName), attachedProcess(process) {}

void BaseScreen::onEnabled()
{
	this->printProcessInfo();
	this->refreshed = false;		// changes applied
}

// added
void BaseScreen::process()
{
	String command;

	// Read user input
	std::cout << "\nroot:\\>";
	std::getline(std::cin, command);

	if (command == "process-smi") {
		this->printProcessSmi();
		this->refreshed = true; // Update after printing
	}
	else if (command == "exit") {
		attachedProcess->manualAddCommand(command);
		attachedProcess->moveToNextLine();
		system("cls");
		ConsoleManager::getInstance()->returnToPreviousConsole();
		ConsoleManager::getInstance()->unregisteredScreen(this->name);
		this->refreshed = true;		// changes applied
	}
	else {
		std::cout << "Unrecognized command: " << command << std::endl;
		std::cout << std::endl;
		attachedProcess->manualAddCommand(command);
		attachedProcess->moveToNextLine();
		this->refreshed = true;		// changes applied
	}
}

void BaseScreen::display()
{
	if (this->refreshed == true) {
		this->printProcessInfo();
	}
}

// added
void BaseScreen::printProcessInfo()
{
	int currIns = this->attachedProcess->getCommandCounter();
	int totalIns = this->attachedProcess->getLinesOfCode();

	std::cout << "\nScreen name: " << this->attachedProcess->getName() << std::endl;
	std::cout << "ID: " << this->attachedProcess->getPID() << std::endl;
	std::cout << "Line: " << currIns << " / " << totalIns << std::endl;

	std::tm arrivalTime = this->attachedProcess->getArrivalTime(); // Store the r-value in a local variable
	std::cout << "Created at: " << std::put_time(&arrivalTime, "%Y-%m-%d %H:%M:%S") << std::endl;
}

// added
void BaseScreen::printProcessSmi()
{
	int currIns = this->attachedProcess->getCommandCounter();
	int totalIns = this->attachedProcess->getLinesOfCode();

	if (currIns != totalIns)
	{
		std::cout << "\nProcess: " << this->attachedProcess->getName() << std::endl;
		std::cout << "ID: " << this->attachedProcess->getPID() << std::endl;

		std::cout << "Current line of instruction: " << currIns << std::endl;
		std::cout << "Total lines of instruction: " << totalIns << std::endl;
	}
	else
	{
		std::cout << "Process: " << this->attachedProcess->getName() << std::endl;
		std::cout << "ID: " << this->attachedProcess->getPID() << std::endl;

		std::cout << "Finished!" << std::endl;
	}

	this->refreshed = false;
}

