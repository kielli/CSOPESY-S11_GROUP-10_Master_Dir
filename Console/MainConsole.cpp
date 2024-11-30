#include "MainConsole.h"

using namespace std;

MainConsole::MainConsole() : AConsole("MainConsole") {}

void MainConsole::onEnabled() {
	this->ASCIITextHeader();
}

void MainConsole::display() {}

void MainConsole::process()
{
	String commandMain;
	bool isValidCommand = false;
	isRunning = true;

	while (isRunning)
	{
		cout << "\nEnter a command: ";
		getline(cin, commandMain);

		// Check if it is a valid initialize command
		if (!this->isInitialized && this->isInitialCommand(commandMain) && this->isRunning)
		{

			if (commandMain == "initialize")
			{
				this->isInitialized = true;
				cout << "Initialization successful." << endl;

				GlobalConfig::getInstance()->printConfig();

				cout << endl;
				continue;	// Skip the rest of the loop
			}
			else if (commandMain == "exit")
			{
				cout << "Exiting the program..." << endl;
				isRunning = false;
				ConsoleManager::getInstance()->exitApplication();
			}
		}

		// Check if the program is initialized before proceeding
		if (this->isInitialized)
		{
			isValidCommand = this->validateCommand(commandMain);

			if (isValidCommand)
			{
				if (commandMain == "initialize")
				{
					cout << "The program is already initialized!" << endl;

					GlobalConfig::getInstance()->printConfig();

					cout << endl;
					continue;
				}
				else if (commandMain == "clear")
				{
					system("cls");
					this->onEnabled();
					continue;	// Skip the rest of the loop
				}
				else if (commandMain == "exit")
				{
					this->isRunning = false;
					cout << "Exiting the program..." << endl;
					ConsoleManager::getInstance()->exitApplication();
					isRunning = false;
				}
				else if (commandMain.substr(0, 6) == "screen")
				{
					if (this->validateScreenCommand(commandMain))
					{
						
						if (commandMain.substr(0, 9) == "screen -s") {
							isRunning = false;
							this->executeScreenSwitchCommand(commandMain);
						}
						
						if (commandMain.substr(0, 9) == "screen -r") {
							isRunning = false;
							this->executeScreenRedrawCommand(commandMain);
						}
						
						if (commandMain.substr(0, 10) == "screen -ls") {
							this->executeScreenListCommand();
						}
					}
					else {
						::cerr << "Error: Invalid screen command.\n";
					}
				}
				else if (commandMain == "scheduler-test") {
					this->executeSchedulerTestCommand();
				}
				else if (commandMain == "scheduler-stop") {
					this->executeSchedulerStopCommand();
				}
				else if (commandMain == "report-util") {
					this->executeReportUtilizationCommand();
				}
				else if (commandMain == "process-smi") { 
					this->displayMainProcessSmi();
				}
				else if (commandMain == "vmstat") {
					this->displayVmstat();
				}
				else {
					this->commandRecognized(commandMain);
				}
			}
			else {
				cerr << "Error: Invalid command. Please enter a valid command.\n" << endl;
			}
		}
		else {
			cerr << "Error: Application is not initialized. Please initialize the application first.\n" << endl;
		}
	}
}

void MainConsole::ASCIITextHeader() const
{
	cout << "  ____    ____      ___     ____    _______    ____    __   __		\n";
	cout << " / ___|  / ___|    / _ \\   |  _ \\   |  ___|   / ___|   \\ \\ / /	\n";
	cout << "| |      \\___ \\   | | | |  | |_) |  |  __|    \\___ \\    \\ V /	\n";
	cout << "| |___    ___) |  | |_| |  |  __/   | |___     ___) |    | |			\n";
	cout << " \\____|  |____/    \\___/   |_|      |_____|   |____/     |_|		\n";
	cout << "______________________________________________________________\n";
	HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_color, 10);
	cout << "Welcome to CSOPESY Emulator!\n";
	cout << "\n";
	displayDevelopers();
	cout << "______________________________________________________________\n";
	SetConsoleTextAttribute(console_color, 14);
	cout << "Type 'exit' to quit, 'clear' to clear the screen, 'initialize' to start the program\n";
	SetConsoleTextAttribute(console_color, 15);
}

void MainConsole::displayDevelopers() const
{
	HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_color, 15);

	cout << "Developers: \n";
	cout << "1. Abenoja, Amelia Joyce \n";
	cout << "2. Cipriaso, James Kielson \n";
	cout << "3. Hallar, Francine Marie \n";
	cout << "4. Hong, Letty \n";
	cout << "5. Pe, Gyan Josh \n";
	cout << "\n";
	cout << "Last Updated: 30-11-2024\n";
}

bool MainConsole::isInitialCommand(String command) const
{
	bool isValid = false;

	String commandList[] = { "initialize", "exit" };

	String inputCommand = command.substr(0, command.find(" "));
	transform(inputCommand.begin(), inputCommand.end(), inputCommand.begin(), ::tolower);

	for (String cmd : commandList) {
		if (inputCommand == cmd) {
			isValid = true;
			break;
		}
	}

	return isValid;
}

bool MainConsole::validateCommand(String command) const
{
	bool isValid = false;

	String commandList[] = { "initialize", "exit", "clear",
							"scheduler-test", "scheduler-stop", "report-util",
							"screen", "process-smi", "vmstat"};

	String inputCommand = command.substr(0, command.find(" "));
	transform(inputCommand.begin(), inputCommand.end(), inputCommand.begin(), ::tolower);

	for (String cmd : commandList) {
		if (inputCommand == cmd) {
			isValid = true;
			break;
		}
	}

	return isValid;
}

void MainConsole::commandRecognized(String command) const
{
	cout << command << " command recognized. Doing something...\n" << endl;
}

bool MainConsole::validateScreenCommand(String command) const
{
	bool isValid = false;

	String subString = command.substr(0, 9);
	String subStringList = command.substr(0, 10);

	if (subString == "screen -s" && command.length() > 9) {
		isValid = true;
	}
	else if (subString == "screen -r" && command.length() > 9) {
		isValid = true;
	}
	
	if (subStringList == "screen -ls" && command.length() == 10) {
		isValid = true;
	}

	if (subString == "screen -s" && command.length() == 9) {
		cerr << "Error: No screen name provided.\n";
	}
	else if (subString == "screen -r" && command.length() == 9) {
		cerr << "Error: No screen name provided.\n";
	}

	return isValid;
}

void MainConsole::executeScreenSwitchCommand(String command) const
{
	String processName = command.substr(10, command.length());

	// Check if the process already exists
	if (ConsoleManager::getInstance()->findExistingProcess(processName)) {
		cerr << "Error: Process " << processName << " already exists.\n";
		return;
	}
	else {
		int newPID = ConsoleManager::getInstance()->getProcessTableSize() + 1;

		shared_ptr<Process> newProcess = make_shared<Process>(newPID, processName, 0, 0);
		shared_ptr<BaseScreen> newScreen = make_shared<BaseScreen>(newProcess, processName);

		ConsoleManager::getInstance()->addProcess(newProcess);

		ConsoleManager::getInstance()->registerScreen(newScreen);
		ConsoleManager::getInstance()->switchToScreen(processName);
		ConsoleManager::getInstance()->process();
		ConsoleManager::getInstance()->drawConsole();
	}
}

void MainConsole::executeScreenRedrawCommand(String command) const
{
	String screenName = command.substr(10, command.length());

	// get the process first
	shared_ptr<Process> currentProcess = ConsoleManager::getInstance()->findProcess(screenName);
	shared_ptr<BaseScreen> currentScreen = make_shared<BaseScreen>(currentProcess, screenName);
	ConsoleManager::getInstance()->registerScreen(currentScreen);
	ConsoleManager::getInstance()->switchToScreen(screenName);
	ConsoleManager::getInstance()->process();
	ConsoleManager::getInstance()->drawConsole();
}

void MainConsole::executeScreenListCommand()
{
	cout << endl;
	for (int i = 0; i < 62; i++) {
		cout << "=";
	}
	cout << endl;
	Scheduler::getInstance()->displaySchedulerStatus();

	cout << endl;
	for (int i = 0; i < 62; i++) {
		cout << "=";
	}
	cout << endl;
}

void MainConsole::executeSchedulerTestCommand()
{
	cout << "Running scheduler test: Start adding processes...\n" << endl;
	this->isStopSchedulerTest = false;

	thread schedulerTestThread([this]() {
		int batchProcessFreq = GlobalConfig::getInstance()->getBatchProcessFreq();
		int cpuCycleCounter = 0;

		while (this->isStopSchedulerTest == false) {
			if (cpuCycleCounter % batchProcessFreq == 0) {
				shared_ptr<Process> newProcess = Scheduler::getInstance()->createUniqueProcess();

				String processName = newProcess->getName();
				if (ConsoleManager::getInstance()->findExistingProcess(processName)) {
					cerr << "Error: Process " << processName << " already exists.\n";
					continue;
				}
				else {
					ConsoleManager::getInstance()->addProcess(newProcess);
				}
			}
			cpuCycleCounter++;

			this_thread::sleep_for(chrono::milliseconds(100));
		}
		});

	schedulerTestThread.detach();
}

void MainConsole::executeSchedulerStopCommand()
{
	cout << "Stopping the scheduler test: Stop adding processes...\n" << endl;
	this->isStopSchedulerTest = true;
	//Scheduler::getInstance()->stopScheduler();
}

void MainConsole::executeReportUtilizationCommand() const
{
	ofstream out("csopesy-log.txt");
	streambuf* coutbuf = cout.rdbuf(); //save old buf
	cout.rdbuf(out.rdbuf()); //redirect cout to out.txt!
	Scheduler::getInstance()->displaySchedulerStatus();
	cout.rdbuf(coutbuf); //reset to standard output again
	cout << "Report generated at csopesy-log.txt\n" << endl;
}

void MainConsole::displayMainProcessSmi()
{
	cout << "\n---------------------------------------------------------------" << endl;
	cout << "| PROCESS-SMI v01.00                 Driver Version: 01.00    |" << endl;
	cout << "---------------------------------------------------------------" << endl;
	cout << "CPU-Util: " << endl;
	cout << "Memory Usage: " << endl;
	cout << "Memory-Util: " << endl;
	cout << "\n";
	cout << "================================================================" << endl;
	cout << "Running processes and memory usage : " << endl;
	cout << "----------------------------------------------------------------" << endl;
	cout << "process-name-holder \tmemory-usage-holder" << endl;
	cout << "process-name-holder \tmemory-usage-holder" << endl;
	cout << "----------------------------------------------------------------" << endl;
}

void MainConsole::displayVmstat()
{
	cout << "\nVMSTAT: " << endl;
	cout << "num-holder \t K Total Memory" << endl;
	cout << "num-holder \t K Used Memory" << endl;
	cout << "num-holder \t K ree Memory" << endl;
	cout << "num-holder \t Idle CPU ticks" << endl;
	cout << "num-holder \t Active CPU ticks" << endl;
	cout << "num-holder \t Total CPU ticks" << endl;
	cout << "num-holder \t Num Paged In" << endl;
	cout << "num-holder \t Num Paged Out" << endl;
}
