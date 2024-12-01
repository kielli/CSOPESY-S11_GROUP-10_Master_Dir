#pragma once
#include <memory>
#include <string>
#include <thread>

#include "../TypeDefRepo.h"
#include "../Process/Process.h"
#include "../Config/GlobalConfig.h"

class CPUCore
{
	public:
		CPUCore();
		~CPUCore() = default;

		void assignProcess(std::shared_ptr<Process> process);

		int getCPUCoreID() const;
		String getProcessName() const;
		int getCommandCounter() const;
		int getLinesOfCode() const;
		std::tm getArrivalTime() const;
		std::shared_ptr<Process> getProcess() const;

		void stop();
		bool isAvailable() const;
		void setAvailable(bool available);

		int getTotalCPUTicks() const;
		int getActiveCPUTicks() const;

	private:
		std::shared_ptr<Process> process = nullptr;
		bool availableFlag = true;
		bool stopFlag = false;
		int cpuCoreID;
		static int nextCPUCoreID;

		int totalCPUTicks;
		int activeCPUTicks;
		int quantumCounter;

		void run();
};
