#pragma once
#include <memory>
#include <queue>
#include <thread>
#include <mutex>

#include "../TypeDefRepo.h"
#include "CPUCore.h"
#include "../Process/Process.h"
#include "../Config/GlobalConfig.h"
#include "../Console/ConsoleManager.h"
#include "../Memory/IMemoryAllocator.h"

class Scheduler
{
public:
	static Scheduler* getInstance();
	static void initialize();
	static void destroy();

	void stopScheduler();

	void startSchedulerThread(String scheduler, int delay, int quantum);

	std::shared_ptr<Process> createUniqueProcess();
	std::shared_ptr<Process> findProcess(String name) const;

	void displaySchedulerStatus();

	void processSMI();
	void vmStat();

    std::shared_ptr<IMemoryAllocator> getMemoryAllocator() const;

private:
	Scheduler();
	~Scheduler() = default;
	Scheduler(Scheduler const&) = delete;
	Scheduler& operator=(Scheduler const&) = delete;
	static Scheduler* sharedInstance;

	int numCPU;
	int bacthProcessFrequency;
	int minInstructions;
	int maxInstructions;

	bool isRunning = false;

	std::vector<std::shared_ptr<Process>> readyQueue;
	std::vector<std::shared_ptr<CPUCore>> cpuCoreList;
	std::vector<std::shared_ptr<Process>> processList;

	std::thread schedulerThread;
	std::mutex schedulerMutex;

	String generateUniqueProcessName(int id);
	void addProcessToReadyQueue(std::shared_ptr<Process> process);

	void runFCFSScheduler(int delay);
	void runRoundRobinScheduler(int delay, int quantum);

	int cycleCounter = 0;

	// Memory Manager
	std::shared_ptr<IMemoryAllocator> memoryAllocator;
	int getTotalCPUTicks();
	int getInactiveCPUTicks();

	friend class FlatMemoryAllocator;
	friend class PagingAllocator;
};
