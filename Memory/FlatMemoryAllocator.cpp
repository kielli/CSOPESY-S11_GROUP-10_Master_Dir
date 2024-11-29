#include "FlatMemoryAllocator.h"

FlatMemoryAllocator::FlatMemoryAllocator(size_t maximumSize) : maximumSize(maximumSize), allocatedSize(0)
{
	this->memory.reserve(maximumSize);
	this->initializeMemory();
}

FlatMemoryAllocator::~FlatMemoryAllocator()
{
	this->memory.clear();
}

void* FlatMemoryAllocator::allocate(size_t size)
{
	// Find the first available block of memory that can accommodate the process
	for (size_t i = 0; i < this->maximumSize - size + 1; ++i) {
		if (!this->allocationMap[i] && this->canAllocateAt(i, size)) {
			this->allocateAt(i, size);
			return &this->memory[i];
		}
	}

	// No available memory block found, return nullptr
	return nullptr;
}

void FlatMemoryAllocator::deallocate(void* ptr)
{
	// Find the index of the memory block to deallocate
	size_t index = static_cast<char*>(ptr) - &this->memory[0];

	if (this->allocationMap[index]) {
		this->deallocateAt(index);
	}
}

String FlatMemoryAllocator::visualizeMemory()
{
	return String(this->memory.begin(), this->memory.end());
}

void FlatMemoryAllocator::initializeMemory()
{
	std::fill(this->memory.begin(), this->memory.end(), '.');	// '.' represents empty/unallocated memory
	std::fill(this->allocationMap.begin(), this->allocationMap.end(), false);
}

bool FlatMemoryAllocator::canAllocateAt(size_t index, size_t size) const
{
	// Check if the memory block is large enough to accommodate the process
	return (index + size <= this->maximumSize);
}

void FlatMemoryAllocator::allocateAt(size_t index, size_t size)
{
	// Mark the memory block as allocated
	//std::fill(this->allocationMap.begin() + index, this->allocationMap.begin() + index + size, true);
	
	for (size_t i = index; i < index + size; ++i) {
		this->allocationMap[i] = true;
	}
	
	this->allocatedSize += size;
}

void FlatMemoryAllocator::deallocateAt(size_t index)
{
	// Mark the memory block as deallocated
	this->allocationMap[index] = false;
}
