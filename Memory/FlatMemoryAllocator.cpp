#include "../Memory/FlatMemoryAllocator.h"
#include "../Process/Process.h"
#include <algorithm>
#include <string>
#include <iostream>

FlatMemoryAllocator::FlatMemoryAllocator(char* baseAddress, size_t maximumSize)
	: baseAddress(baseAddress), maximumSize(maximumSize), allocatedSize(0) {
	allocationMap.resize(maximumSize, false);
}

FlatMemoryAllocator::~FlatMemoryAllocator() {
	// Clear allocated map
	allocationMap.clear();
}

void* FlatMemoryAllocator::allocate(std::shared_ptr<Process> process) {
	//changed getMemorySize 
	if (process->getRequiredMemory() == 0 || process->getRequiredMemory() > maximumSize) {
		return nullptr; // Invalid request
	}
	for (size_t i = 0; i <= maximumSize - process->getRequiredMemory(); i++) {
		if (!allocationMap[i] && canAllocateAt(i, process->getRequiredMemory())) {
			allocateAt(i, process->getRequiredMemory());
			//std::cout << "Allocated " << process->getRequiredMemory() << " bytes at index " << i << ".\n";   //check
			return this->baseAddress + i;
		}
	}
	//std::cout << "Allocation failed for size " << process->getRequiredMemory() << ".\n";  //check
	return nullptr;
}


void FlatMemoryAllocator::deallocate(std::shared_ptr<Process> process) {
	if (process->getStoredAt() == nullptr || process->getRequiredMemory() == 0) {
		//std::cerr << "Invalid deallocation request.\n"; //check
		return;
	}
	size_t index = static_cast<char*>(process->getStoredAt()) - this->baseAddress;
	if (index + process->getRequiredMemory() > maximumSize || !allocationMap[index]) {
		//std::cerr << "Invalid deallocation attempt at index " << index << ".\n"; //check
		return;
	}
	deallocateAt(index, process->getRequiredMemory());
	//std::cout << "Memory deallocated at index " << index << " for size " << process->getRequiredMemory() << ".\n"; //check
}

void FlatMemoryAllocator::visualizeMemory() const {
}


bool FlatMemoryAllocator::isFull() const {
	return allocatedSize >= maximumSize;
}

bool FlatMemoryAllocator::canAllocateAt(size_t index, size_t size) const {
	for (size_t i = 0; i < size; i++) {
		if (allocationMap[index + i]) {
			return false;
		}
	}
	return true;
}

void FlatMemoryAllocator::allocateAt(size_t index, size_t size) {
	std::fill(allocationMap.begin() + index, allocationMap.begin() + index + size, true);
	allocatedSize += size;
}

void FlatMemoryAllocator::deallocateAt(size_t index, size_t size) {
	std::fill(allocationMap.begin() + index, allocationMap.begin() + index + size, false);
	allocatedSize -= size;
}
