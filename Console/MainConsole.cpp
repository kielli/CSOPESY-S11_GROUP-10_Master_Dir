#include "MainConsole.h"

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
		std::cout << "\nEnter a command: ";
		std::getline(std::cin, commandMain);

		// Check if it is a valid initialize command
		if (!this->isInitialized && this->isInitialCommand(commandMain) && this->isRunning)
		{
			if (commandMain == "initialize")
			{
				this->isInitialized = true;
				std::cout << "Initialization successful." << std::endl;

				GlobalConfig::getInstance()->printConfig();

				std::cout << std::endl;
				continue;	// Skip the rest of the loop
			}
			else if (commandMain == "exit")
			{
				std::cout << "Exiting the program..." << std::endl;
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
					std::cout << "The program is already initialized!" << std::endl;

					GlobalConfig::getInstance()->printConfig();

					std::cout << std::endl;
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
					std::cout << "Exiting the program..." << std::endl;
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
						std::cerr << "Error: Invalid screen command.\n";
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
				else if (commandMain == "process-smi") { //added
					this->displayMainProcessSmi();
				}
				else if (commandMain == "vmstat") { //added
					this->displayVmstat();
				}
				else {
					this->commandRecognized(commandMain);
				}
			}
			else {
				std::cerr << "Error: Invalid command. Please enter a valid command.\n" << std::endl;
			}
		}
		else {
			std::cerr << "Error: Application is not initialized. Please initialize the application first.\n" << std::endl;
		}
	}
}

void MainConsole::ASCIITextHeader() const
{
	std::cout << "  ____    ____      ___     ____    _______    ____    __   __		\n";
	std::cout << " / ___|  / ___|    / _ \\   |  _ \\   |  ___|   / ___|   \\ \\ / /	\n";
	std::cout << "| |      \\___ \\   | | | |  | |_) |  |  __|    \\___ \\    \\ V /	\n";
	std::cout << "| |___    ___) |  | |_| |  |  __/   | |___     ___) |    | |			\n";
	std::cout << " \\____|  |____/    \\___/   |_|      |_____|   |____/     |_|		\n";
	std::cout << "______________________________________________________________\n";
	HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_color, 10);
	std::cout << "Welcome to CSOPESY Emulator!\n";
	std::cout << "\n";
	displayDevelopers();
	std::cout << "______________________________________________________________\n";
	SetConsoleTextAttribute(console_color, 14);
	std::cout << "Type 'exit' to quit, 'clear' to clear the screen, 'initialize' to start the program\n";
	SetConsoleTextAttribute(console_color, 15);
}

void MainConsole::displayDevelopers() const
{
	HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_color, 15);

	std::cout << "Developers: \n";
	std::cout << "1. Abenoja, Amelia Joyce \n";
	std::cout << "2. Cipriaso, James Kielson \n";
	std::cout << "3. Hallar, Francine Marie \n";
	std::cout << "4. Hong, Letty \n";
	std::cout << "5. Pe, Gyan Josh \n";
	std::cout << "\n";
	std::cout << "Last Updated: 01-12-2024\n";
}

bool MainConsole::isInitialCommand(String command) const
{
	bool isValid = false;

	String commandList[] = { "initialize", "exit" };

	String inputCommand = command.substr(0, command.find(" "));
	std::transform(inputCommand.begin(), inputCommand.end(), inputCommand.begin(), ::tolower);

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
							"screen", "process-smi", "vmstat",
							"dummy-layout", "marquee-console", "marquee-console" };

	String inputCommand = command.substr(0, command.find(" "));
	std::transform(inputCommand.begin(), inputCommand.end(), inputCommand.begin(), ::tolower);

	for (String cmd : commandList) {
		if (inputCommand == cmd) {
			isValid = true;
			break;
		}
	}

	return isValid;
}

void MainConsole::commandRecognized(String command) const {
	std::cout << command << " command recognized. Doing something...\n" << std::endl;
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
		std::cerr << "Error: No screen name provided.\n";
	}
	else if (subString == "screen -r" && command.length() == 9) {
		std::cerr << "Error: No screen name provided.\n";
	}

	return isValid;
}

void MainConsole::executeScreenSwitchCommand(String command) const
{
	String processName = command.substr(10, command.length());

	// Check if the process already exists
	if (ConsoleManager::getInstance()->findExistingProcess(processName)) {
		std::cerr << "Error: Process " << processName << " already exists.\n";
		return;
	}
	else {
		int newPID = ConsoleManager::getInstance()->getProcessTableSize() + 1;

		std::shared_ptr<Process> newProcess = std::make_shared<Process>(newPID, processName, 0);
		std::shared_ptr<BaseScreen> newScreen = std::make_shared<BaseScreen>(newProcess, processName);

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
	std::shared_ptr<Process> currentProcess = ConsoleManager::getInstance()->findProcess(screenName);
	std::shared_ptr<BaseScreen> currentScreen = std::make_shared<BaseScreen>(currentProcess, screenName);
	ConsoleManager::getInstance()->registerScreen(currentScreen);
	ConsoleManager::getInstance()->switchToScreen(screenName);
	ConsoleManager::getInstance()->process();
	ConsoleManager::getInstance()->drawConsole();
}

void MainConsole::executeScreenListCommand()
{
	std::cout << std::endl;
	for (int i = 0; i < 62; i++) {
		std::cout << "=";
	}
	std::cout << std::endl;
	Scheduler::getInstance()->displaySchedulerStatus();

	std::cout << std::endl;
	for (int i = 0; i < 62; i++) {
		std::cout << "=";
	}
	std::cout << std::endl;
}

void MainConsole::executeSchedulerTestCommand()
{
	std::cout << "Running scheduler test: Start adding processes...\n" << std::endl;
	this->isStopSchedulerTest = false;

	std::thread schedulerTestThread([this]()
	{
		int batchProcessFreq = GlobalConfig::getInstance()->getBatchProcessFreq();
		int cpuCycleCounter = 0;

		while (this->isStopSchedulerTest == false) {
			if (cpuCycleCounter % batchProcessFreq == 0) {
				std::shared_ptr<Process> newProcess = Scheduler::getInstance()->createUniqueProcess();

				String processName = newProcess->getName();
				if (ConsoleManager::getInstance()->findExistingProcess(processName)) {
					std::cerr << "Error: Process " << processName << " already exists.\n";
					continue;
				}
				else {
					ConsoleManager::getInstance()->addProcess(newProcess);
				}
			}
			cpuCycleCounter++;

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	});

	schedulerTestThread.detach();
}

void MainConsole::executeSchedulerStopCommand()
{
	std::cout << "Stopping the scheduler test: Stop adding processes...\n" << std::endl;
	this->isStopSchedulerTest = true;
	//Scheduler::getInstance()->stopScheduler();
}

void MainConsole::executeReportUtilizationCommand() const
{
	std::ofstream out("csopesy-log.txt");
	std::streambuf* coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
	Scheduler::getInstance()->displaySchedulerStatus();
	std::cout.rdbuf(coutbuf); //reset to standard output again
	std::cout << "Report generated at csopesy-log.txt\n" << std::endl;
}

// added
void MainConsole::displayMainProcessSmi()
{
	size_t memTotal = GlobalConfig::getInstance()->getMaxOverallMem();
	size_t memUsed = MemoryManager::getInstance()->currentMemAllocated();
	float memUtil = static_cast<float>(memUsed) / static_cast<float>(memTotal) * 100;

	std::vector<std::shared_ptr<Process>> memProcessList = MemoryManager::getInstance()->getProcessList();

	std::cout << "\n---------------------------------------------------------------" << std::endl;
	std::cout << "| PROCESS-SMI v01.00                 Driver Version: 01.00    |" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "CPU-Util: " << Scheduler::getInstance()->getCPUUtilization() << "%" << std::endl;
	std::cout << "Memory Usage: " << memUsed << std::endl;
	std::cout << "Memory-Util: "<< memUtil  << "%" << std::endl;
	std::cout << "\n";
	std::cout << "================================================================" << std::endl;
	std::cout << "Running processes and memory usage : " << std::endl;
	std::cout << "----------------------------------------------------------------" << std::endl;
	for(std::shared_ptr<Process> process : memProcessList){
		/*std::cout << process->getName() << " : " << process->getMemoryRequired() << std::endl;*/
		if (!process->isFinished()) {
			std::cout << process->getName() << " : " << process->getMemoryRequired() << std::endl;
		}
	}
	std::cout << "----------------------------------------------------------------" << std::endl;
}

// added
void MainConsole::displayVmstat()
{
	size_t memTotal = GlobalConfig::getInstance()->getMaxOverallMem();
	size_t memUsed = MemoryManager::getInstance()->currentMemAllocated();
	size_t memFree = MemoryManager::getInstance()->getFreeMemory();

	int activeCPUTicks = Scheduler::getInstance()->getActiveCPUTicks();
	int totalCPUTicks = Scheduler::getInstance()->getTotalCPUTicks();
	int idleCPUTicks = totalCPUTicks - activeCPUTicks;
	
	size_t pagedIn = MemoryManager::getInstance()->getPagedIn();
	size_t pagedOut = MemoryManager::getInstance()->getPagedOut();

	std::cout << "\nVMSTAT: " 	<< std::endl;
	std::cout << memTotal 		<< " \t K Total Memory" << std::endl;
	std::cout << memUsed 		<< " \t K Used Memory" << std::endl;
	std::cout << memFree 		<< " \t K Free Memory" << std::endl;
	std::cout << idleCPUTicks 	<< " \t Idle CPU ticks" << std::endl;
	std::cout << activeCPUTicks << " \t Active CPU ticks" << std::endl;
	std::cout << totalCPUTicks 	<< " \t Total CPU ticks" << std::endl;
	std::cout << pagedIn 		<< " \t Num Paged In" << std::endl;
	std::cout << pagedOut 		<< " \t Num Paged Out" << std::endl;
}
