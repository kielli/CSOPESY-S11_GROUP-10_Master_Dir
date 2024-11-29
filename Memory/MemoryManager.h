#pragma once
#include "Process.h"
#include <vector>
#include <iostream>
#include <optional>
#include <fstream>
#include <string>
#include <memory>
#include <algorithm>
#include <cmath>
#include <map>
#include <mutex>
#include <unordered_map>

#include "../TypeDefRepo.h"
#include "../Config/GlobalConfig.h"
#include "../Process/Process.h"
#include "IMemoryAllocator.h"
#include "FlatMemoryAllocator.h"
#include "PagingAllocator.h"

class MemoryManager
{
public:
	static MemoryManager* getInstance();
	static void initialize();
	static void destroy();

	void displayMemoryStatus();
	void displayBackingStore();

	void exportMemoryStatusToFile(int quantum);

	size_t getUsedMemory() const;

	size_t calculateExternalFragmentation() const;

	int getPagedInCount() const;
	int getPagedOutCount() const;

	bool isMemoryFull() const;

	void addProcessToMemory(std::shared_ptr<Process> process);
	void removeProcessFromMemory(std::shared_ptr<Process> process);

	bool backingStoreContains(std::shared_ptr<Process> process) const;
	void addPageToBackingStore(std::shared_ptr<Process> process);
	void removePageFromBackingStore(std::shared_ptr<Process> process);

	void executeBackingStoreOperation(std::shared_ptr<Process> process);

	bool containsPage(std::shared_ptr<Process> process) const;

	Process::RequirementFlags generateRequirements();

	size_t getMaxMemory() const;
	
	void setMaxMemory(size_t maxMemory);
	void setMemoryPerFrame(size_t memoryPerFrame);
	void setMinMemoryPerProcess(size_t minMemoryPerProcess);
	void setMaxMemoryPerProcess(size_t maxMemoryPerProcess);
	void setMemoryAllocator();

	size_t frameCount;

	size_t getMemoryPerFrame() const;

private:
	MemoryManager();
	~MemoryManager() = default;
	MemoryManager(MemoryManager const&) = delete;
	MemoryManager& operator=(MemoryManager const&) = delete;
	static MemoryManager* sharedInstance;

	char* baseAddress;
	size_t maxMemory;
	size_t memoryPerFrame;
	size_t minMemoryPerProcess;
	size_t maxMemoryPerProcess;

	int pageInCount;
	int pageOutCount;

	std::unordered_map<int, std::shared_ptr<Process>> backingStore;
	std::map<int, std::shared_ptr<Process>> timeMap;
	int processCount;

	std::mutex memoryMutex;

	std::unique_ptr<IMemoryAllocator> memoryAllocator;

	std::vector<std::shared_ptr<Process>> processListInMemory;

	String updateTimeStamp();

	void allocateMemory(std::shared_ptr<Process> process);
	void deallocateMemory(std::shared_ptr<Process> process);

	void sortMemoryDisplay();

	void readFromFile();
	void writeToFile();
};

