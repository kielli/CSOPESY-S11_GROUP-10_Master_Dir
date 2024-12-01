#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include "IMemoryAllocator.h"

class FlatMemoryAllocator : public IMemoryAllocator {
    public:
        FlatMemoryAllocator(size_t maxMemSize);
        ~FlatMemoryAllocator();

        size_t allocate(std::shared_ptr<Process> process) override;
        void deallocate(std::shared_ptr<Process> process) override;
        size_t getAllocatedMem();
		bool isMemFull() const;

    private:
        size_t maxMemSize;
        size_t allocatedMem;
        size_t memBaseAddress;
        std::vector<bool> allocationMap;

        bool canAllocate(size_t index, size_t size) const;
        void allocateAt(size_t index, size_t size);
        void deallocateAt(size_t index, size_t size);
};