#include "ConsoleManager.h"
#include "MainConsole.h"

using namespace std;

// Constructor
ConsoleManager::ConsoleManager()
{
	this->running = true;

	// Initialize consoles
	this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	const shared_ptr<MainConsole> mainConsole = make_shared<MainConsole>();

	this->consoleTable[MAIN_CONSOLE_NAME] = mainConsole;
	this->switchConsole(MAIN_CONSOLE_NAME);
}

ConsoleManager* ConsoleManager::sharedInstance = nullptr;

ConsoleManager* ConsoleManager::getInstance()
{
	if (sharedInstance == nullptr) {
		sharedInstance = new ConsoleManager();
	}
	return sharedInstance;
}

void ConsoleManager::initialize() {
	sharedInstance = new ConsoleManager();
}

void ConsoleManager::destroy() {
	delete sharedInstance;
}

void ConsoleManager::drawConsole() const
{
	if (this->currentConsole != nullptr) {
		this->currentConsole->display();
	}
	else {
		cout << "There is no assigned console. Please check." << endl;
	}
}

void ConsoleManager::process() const
{
	if (this->currentConsole != nullptr) {
		this->currentConsole->process();
	}
	else {
		cout << "There is no assigned console. Please check." << endl;
	}
}

void ConsoleManager::switchConsole(String consoleName)
{
	if (this->consoleTable.contains(consoleName)) {
		// Clear the screen first
		system("cls");
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[consoleName];
		this->currentConsole->onEnabled();
	}
	else {
		cerr << "Console name " << consoleName << " is not found. Was it initialized?" << endl;
	}
}

void ConsoleManager::registerScreen(shared_ptr<BaseScreen> screenRef)
{
	if (this->consoleTable.contains(screenRef->getName())) {
		cerr << "Screen name " << screenRef->getName() << " is already registered. Please use a different name" << endl;
		return;
	}

	this->consoleTable[screenRef->getName()] = screenRef;
}

void ConsoleManager::switchToScreen(String screenName)
{
	if (this->consoleTable.contains(screenName)) {
		// Clear the screen first
		system("cls");
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[screenName];
		this->currentConsole->onEnabled();
	}
	else {
		cerr << "Screen name " << screenName << " is not found. Was it initialized?" << endl;
	}
}

void ConsoleManager::unregisteredScreen(String screenName)
{
	if (this->consoleTable.contains(screenName)) {
		this->consoleTable.erase(screenName);
	}
	else {
		cerr << "Unable to unregister screen " << screenName << ". Was it registered?" << endl;
	}
}

void ConsoleManager::returnToPreviousConsole()
{
	if (this->previousConsole != nullptr) {
		this->currentConsole = this->previousConsole;
		this->previousConsole = nullptr;
		this->currentConsole->onEnabled();
	}
	else {
		cerr << "There is no previous console to return to." << endl;
	}
}

void ConsoleManager::exitApplication() {
	this->running = false;
}

bool ConsoleManager::isRunning() const {
	return this->running;
}

HANDLE ConsoleManager::getConsoleHandle() const {
	return this->consoleHandle;
}

void ConsoleManager::setCursorPosition(int posX, int posY) const {
	COORD coord;
	coord.X = posX;
	coord.Y = posY;
	SetConsoleCursorPosition(this->consoleHandle, coord);
}

void ConsoleManager::addProcess(shared_ptr<Process> process) {
	int id = processTable.size() + 1;

	this->processTable[id] = process;
}

shared_ptr<Process> ConsoleManager::findProcess(String processName) {
	// Iterate through the processTable
	for (const auto& [id, process] : processTable) {
		// Check if the process name matches
		if (process->getName() == processName) {
			return process; // Return the matching process
		}
	}
	// If no process is found, return a null shared_ptr
	return nullptr;
}

int ConsoleManager::getProcessTableSize() const {
	return this->processTable.size();
}

bool ConsoleManager::findExistingProcess(String name)
{
	bool isExist = false;

	// Iterate through the processTable
	for (const auto& [id, process] : processTable) {
		// Check if the process name matches
		if (process->getName() == name) {
			isExist = true;
			break;
		}
	}

	return isExist;
}

ConsoleManager::ProcessTable ConsoleManager::getProcessTable() const {
	return this->processTable;
}