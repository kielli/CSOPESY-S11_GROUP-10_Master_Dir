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
#include "../Memory/FlatMemoryAllocator.h"
#include "../Scheduler/BackingStore.h"


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
	//added
	void removeFinishedProcesses();
	void reallocateMemoryInWaitList();

private:
	Scheduler();
	void swapOutProcess();
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

	//added
	std::vector<std::shared_ptr<Process>> waitListQueue;  //waiting to be allocated

	std::thread schedulerThread;
	std::mutex schedulerMutex;

	String generateUniqueProcessName(int id);
	void addProcessToReadyQueue(std::shared_ptr<Process> process);

	void runFCFSScheduler(int delay);
	void runRoundRobinScheduler(int delay, int quantum);

	//added
	std::unique_ptr<FlatMemoryAllocator> memoryAllocator;
	size_t overAllMemory;
	size_t memPerFrame;
	size_t memPerProc;
	char* baseAddress;
	std::unique_ptr<BackingStore> backingStore;
};

