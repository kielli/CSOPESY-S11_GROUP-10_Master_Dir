#include "DummyProcessLayout.h"

void DummyProcessLayout::displayCurrentDateTimes() const
{
	// Format:
	// Day Month Date HH:MM:SS Year
	// Example: Mon Jan 1 00:00:00 2021

	// Get the current time
	time_t now = time(0);
	tm localTime;
	localtime_s(&localTime, &now);

	// Convert the time to a string
	char buffer[80];
	strftime(buffer, 80, "%a %b %d %H:%M:%S %Y", &localTime);

	// Display the current date and time
	std::cout << buffer << std::endl;
}

void DummyProcessLayout::displayGPUSummaries() const
{
	std::cout << "+-----------------------------------------------------------------------------------------+" << std::endl;
	std::cout << "| NVIDIA-SMI 551.86                 Driver Version: 551.86         CUDA Version: 12.4     |" << std::endl;
	std::cout << "|-----------------------------------------+------------------------+----------------------+" << std::endl;
	std::cout << "| GPU  Name                     TCC/WDDM  | Bus-Id          Disp.A | Volatile Uncorr. ECC |" << std::endl;
	std::cout << "| Fan  Temp   Perf          Pwr:Usage/Cap |           Memory-Usage | GPU-Util  Compute M. |" << std::endl;
	std::cout << "|                                         |                        |               MIG M. |" << std::endl;
	std::cout << "==========================================+========================+=======================" << std::endl;
	std::cout << "|   0  NVIDIA GeForce GTX 1080      WDDM  |   00000000:26:00.0  On |                  N/A |" << std::endl;
	std::cout << "| 28%   37C    P8             11W /  180W |     701MiB /   8192MiB |      0%      Default |" << std::endl;
	std::cout << "+-----------------------------------------+------------------------+----------------------+" << std::endl;
}

void DummyProcessLayout::displayProcessList(const std::vector<GPUProcess>& processes) const
{
	std::cout << "+-----------------------------------------------------------------------------------------+" << std::endl;
	std::cout << "| Processes:                                                                              |" << std::endl;
	std::cout << "|  GPU   GI   CI        PID   Type   Process name                              GPU Memory |" << std::endl;
	std::cout << "|        ID   ID                                                               Usage      |" << std::endl;
	std::cout << "|=========================================================================================|" << std::endl;

	for (const auto& process : processes) {
		std::cout << "|    " << process.gpu << "   " << std::setw(3) << process.giid << "  " << std::setw(3) << process.ciid << "      "
			<< std::setw(4) << process.pid
			<< "    " << std::setw(3) << process.type
			<< "   " << std::left << std::setw(38) << process.processName   // Add std::left here for left alignment
			<< "      " << std::right << std::setw(3) << process.memoryUsage  // Return to std::right for numbers
			<< "      |" << std::endl;
	}

	std::cout << "+-----------------------------------------------------------------------------------------+" << std::endl;
}
