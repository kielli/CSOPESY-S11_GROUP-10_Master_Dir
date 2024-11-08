#include "FlatMemoryAllocator.h"
FlatMemoryAllocator::FlatMemoryAllocator(size_t maximumSize) : maximumSize(maximumSize), allocatedSize(0) {
	memory.reserve(maximumSize);
	initializeMemory();
}
FlatMemoryAllocator::~FlatMemoryAllocator() {
	memory.clear();
}

void* FlatMemoryAllocator:: allocate(size_t size){
	for (size_t i = 0; i < maximumSize - size + 1; i++) {
		if (!allocationMap[i] && canAllocateAt(i, size)) {
			allocateAt(i, size);
			return &memory[i];
		}
	}
	return nullptr;
}
void FlatMemoryAllocator::deallocate(void* ptr){
	size_t index = static_cast<char*>(ptr) - &memory[0];
	if (allocationMap[index]) {
		deallocateAt(index);
	}
}

string FlatMemoryAllocator::visualizeMemory() {
	return string(memory.begin(), memory.end());
}