#include "BaseScreen.h"

#include "../Console/ConsoleManager.h"

using namespace std;

BaseScreen::BaseScreen(shared_ptr<Process> process, String processName) : AConsole(processName), attachedProcess(process)
{}

void BaseScreen::onEnabled()
{
	this->printProcessInfo();
	this->refreshed = false;		// changes applied
}

void BaseScreen::process()
{
	cout << "\nroot:\\>";

	// Read user input
	String command;
	getline(cin, command);

	if (command == "clear" || command == "cls") {
		attachedProcess->manualAddCommand(command);
		attachedProcess->moveToNextLine();
		this->refreshed = true;		// changes applied
		system("cls");
	}
	else if (command == "process-smi") {
		this->printProcessInfo();
		this->refreshed = true; // Update after printing
	}
	//else if (command == "print") {
	//	// Log the process print command
	//	this->refreshed = false; // Mark for refresh after state changes
	//	attachedProcess->manualAddCommand(command);
	//}
	else if (command == "exit") {
		attachedProcess->manualAddCommand(command);
		attachedProcess->moveToNextLine();
		system("cls");
		ConsoleManager::getInstance()->returnToPreviousConsole();
		ConsoleManager::getInstance()->unregisteredScreen(this->name);
		this->refreshed = true;		// changes applied
	}
	else {
		cout << "Unrecognized command: " << command << endl;
		cout << endl;
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

void BaseScreen::printProcessInfo()
{
	cout << "Process Name: " << this->attachedProcess->getName() << endl;
	cout << "Current line of instruction / Total lines of instruction: "
		<< this->attachedProcess->getCommandCounter() << " / "
		<< this->attachedProcess->getLinesOfCode() << endl;

	tm arrivalTime = this->attachedProcess->getArrivalTime(); // Store the r-value in a local variable
	cout << "Timestamp: " << put_time(&arrivalTime, "%Y-%m-%d %H:%M:%S") << endl;

	this->refreshed = false;
}