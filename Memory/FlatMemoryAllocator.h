#pragma once

#include <memory>
#include <vector>
#include <utility>
#include <ctime>
#include <iostream>
#include <cstdlib> 
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <iomanip>

#include "../TypeDefRepo.h"
#include "IMemoryAllocator.h"
#include "../Process/Process.h"
#include "../Scheduler/Scheduler.h"

class FlatMemoryAllocator : public IMemoryAllocator {
public:
	FlatMemoryAllocator(int maxOverAllMemory);
	~FlatMemoryAllocator() = default;

	bool allocate(std::shared_ptr<Process> process) override;
	void deallocate(std::shared_ptr<Process> process) override;
	void printProcesses() override;
	void vmstat() override;

    void saveBackingStore(const std::unordered_map<int, std::pair<int, int>>& backingStore);

private:
    void readBackingStore(std::shared_ptr<Process> process);
    void writeBackingStore(std::shared_ptr<Process> process);
    std::unordered_map<int, std::pair<int, int>> loadBackingStore();
    int maxOverAllMemory;
    std::vector<std::pair<std::shared_ptr<Process>, std::pair<int, int>>> memory;

    // Add new members
    size_t totalFragmentation{0};
    std::mutex memoryMutex;
    static constexpr size_t DEFRAG_THRESHOLD = 0.3; // 30% fragmentation threshold
};