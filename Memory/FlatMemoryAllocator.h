#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include "IMemoryAllocator.h"

class FlatMemoryAllocator : public IMemoryAllocator {
    public:
        FlatMemoryAllocator(char* memBaseAddress, size_t maxMemSize);
        ~FlatMemoryAllocator();

        void* allocate(Process* process) override;
        void deallocate(Process* process) override;
		bool isMemFull() const;

    private:
        size_t maxMemSize;
        size_t allocatedMem;
        char* memBaseAddress;
        std::vector<bool> allocationMap;

        bool canAllocate(size_t index, size_t size) const;
        void allocateAt(size_t index, size_t size);
        void deallocateAt(size_t index, size_t size);
};