#include "FlatMemoryAllocator.h"
#include <chrono>
#include <iomanip>
#include <ctime>
FlatMemoryAllocator::FlatMemoryAllocator(size_t maximumSize) : maximumSize(maximumSize), allocatedSize(0) {
    memory.reserve(maximumSize);
    initializeMemory();
}

FlatMemoryAllocator::~FlatMemoryAllocator() {
    memory.clear();
}

void* FlatMemoryAllocator::allocate(size_t size) {
    for (size_t i = 0; i < maximumSize - size + 1; i++) {
        if (!allocationMap[i] && canAllocateAt(i, size)) {
            allocateAt(i, size);
            return &memory[i];
        }
    }
    return nullptr;
}

void FlatMemoryAllocator::deallocate(void* ptr) {
    size_t index = static_cast<char*>(ptr) - &memory[0];
    if (allocationMap[index]) {
        deallocateAt(index);
    }
}

string FlatMemoryAllocator::visualizeMemory() {
    return string(memory.begin(), memory.end());
}

size_t FlatMemoryAllocator::externalFragmentation() const {
    size_t fragmentation = 0;
    bool inFragment = false;

    for (size_t i = 0; i < maximumSize; ++i) {
        if (!allocationMap.at(i)) {  // Free memory block
            if (!inFragment) {
                inFragment = true;
            }
        } else {  // Allocated block
            if (inFragment) {
                fragmentation++;
                inFragment = false;
            }
        }
    }

    return fragmentation;
}

void FlatMemoryAllocator::writeReportToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        // Write timestamp
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        
        tm finish_local_time;
        localtime_s(&finish_local_time, &now_c);
        // Write timestamp with chrono and put_time
        file << "Timestamp: " << std::put_time(&finish_local_time, "%Y-%m-%d %H:%M:%S") << "\\n";

        // Write number of allocated blocks (processes in memory)
        file << "Number of processes in memory: " << allocatedSize << "\\n";

        // Write external fragmentation in KB
        file << "External fragmentation: " << externalFragmentation() * 1024 << " KB\\n";

        // Print memory state
        file << "Memory state: " << visualizeMemory() << "\\n\\n";
    }
    file.close();
}