#include "FlatMemoryAllocator.h"

FlatMemoryAllocator::FlatMemoryAllocator(char* baseAddress, size_t maximumSize)
	: baseAddress(baseAddress), maximumSize(maximumSize), allocatedSize(0) {
	allocationMap.resize(maximumSize, false);
}

FlatMemoryAllocator::~FlatMemoryAllocator() {
	// Clear allocated map
	allocationMap.clear();
}

void* FlatMemoryAllocator::allocate(std::shared_ptr<Process> process) {
	if (process->getMemorySize() == 0 || process->getMemorySize() > maximumSize) {
		return nullptr; // Invalid request
	}
	for (size_t i = 0; i <= maximumSize - process->getMemorySize(); i++) {
		if (!allocationMap[i] && canAllocateAt(i, process->getMemorySize())) {
			allocateAt(i, process->getMemorySize());
			//std::cout << "Allocated " << size << " bytes at index " << i << ".\n";
			return this->baseAddress + i;
		}
	}
	//std::cout << "Allocation failed for size " << process->getMemorySize() << ".\n";
	return nullptr;
}


void FlatMemoryAllocator::deallocate(std::shared_ptr<Process> process) {
	if (process->getBaseAddress() == nullptr || process->getMemorySize() == 0) {
		//std::cerr << "Invalid deallocation request.\n";
		return;
	}
	size_t index = static_cast<char*>(process->getBaseAddress()) - this->baseAddress;
	if (index + process->getMemorySize() > maximumSize || !allocationMap[index]) {
		//std::cerr << "Invalid deallocation attempt at index " << index << ".\n";
		return;
	}
	deallocateAt(index, process->getMemorySize());
	//std::cout << "Memory deallocated at index " << index << " for size " << size << ".\n";
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