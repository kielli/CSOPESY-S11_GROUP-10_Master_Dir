#ifndef FLATMEMORYALLOCATOR_H
#define FLATMEMORYALLOCATOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <ctime>
#include "MemoryAllocator.h"

using namespace std;

class FlatMemoryAllocator : public MemoryAllocator {
private:
    size_t maximumSize;
    size_t allocatedSize;
    vector<char> memory;
    unordered_map<size_t, bool> allocationMap;

    void initializeMemory() {
        memory.assign(maximumSize, '.');
        for (size_t i = 0; i < maximumSize; i++) {
            allocationMap[i] = false;
        }
    }

    bool canAllocateAt(size_t index, size_t size) const {
        if (index + size > maximumSize) return false;
        for (size_t i = index; i < index + size; i++) {
            if (allocationMap.at(i)) return false;
        }
        return true;
    }

    void allocateAt(size_t index, size_t size) {
        if (index + size <= maximumSize) {
            for (size_t i = index; i < index + size; i++) {
                allocationMap[i] = true;
                memory[i] = 'X';
            }
            allocatedSize += size;
        }
    }

    void deallocateAt(size_t index) {
        allocationMap[index] = false;
        memory[index] = '.';
        allocatedSize--;
    }

    size_t externalFragmentation() const;  // Declaration for fragmentation calculation

public:
    FlatMemoryAllocator(size_t maximumSize);
    ~FlatMemoryAllocator();

    void* allocate(size_t size) override;
    void deallocate(void* ptr) override;
    string visualizeMemory() override;

    void writeReportToFile(const std::string& filename);  // Declaration for report writing
};
#endif