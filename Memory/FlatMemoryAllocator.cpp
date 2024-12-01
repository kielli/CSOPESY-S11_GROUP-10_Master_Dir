#include "FlatMemoryAllocator.h"
#include <iostream>
#include <vector>
#include <unordered_map>

FlatMemoryAllocator::FlatMemoryAllocator(size_t maxMemSize) : maxMemSize(maxMemSize), allocatedMem(0), memBaseAddress(0){
    allocationMap.resize(maxMemSize, false);
}

FlatMemoryAllocator::~FlatMemoryAllocator(){
    allocationMap.clear();
}

size_t FlatMemoryAllocator::allocate(std::shared_ptr<Process> process)
{
    if(process->getMemoryRequired() == 0 || process->getMemoryRequired() > maxMemSize)
        return -1;
        
    for(size_t i = 0; i < maxMemSize - process->getMemoryRequired(); i++) {
        if(canAllocate(i, process->getMemoryRequired())) {
            allocateAt(i, process->getMemoryRequired());
            process->setMemBaseAddress(i);  // Set the actual base address
            return i;  // Return actual base address
        }
    }
    return -1;
}

void FlatMemoryAllocator::deallocate(std::shared_ptr<Process> process)
{
    if(process->getMemBaseAddress() == -1 || process->getMemoryRequired() == 0)
        return;
    size_t index = process->getMemBaseAddress();
    if(!allocationMap[index] && process->getMemoryRequired()+index > maxMemSize)
        return;
    deallocateAt(index, process->getMemoryRequired());
}

bool FlatMemoryAllocator::canAllocate(size_t index, size_t size) const
{
    for (size_t i = 0; i < size; i++) {
		if (allocationMap[index + i]) {
			return false;
		}
	}
	return true;
}

void FlatMemoryAllocator::allocateAt(size_t index, size_t size) {
    std::fill(allocationMap.begin() + index, allocationMap.begin() + index + size, true);
    allocatedMem+=size;
}

void FlatMemoryAllocator::deallocateAt(size_t index, size_t size) {
    std::fill(allocationMap.begin() + index, allocationMap.begin() + index + size, false);
    allocatedMem-=size;
}

size_t FlatMemoryAllocator::getAllocatedMem() {
    return allocatedMem;
}

bool FlatMemoryAllocator::isMemFull() const {
	return allocatedMem >= maxMemSize;
}