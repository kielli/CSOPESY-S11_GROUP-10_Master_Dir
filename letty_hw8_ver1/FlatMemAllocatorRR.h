#ifndef FLAT_MEM_ALLOCATOR_RR_H
#define FLAT_MEM_ALLOCATOR_RR_H

#include "MemoryAllocator.h"

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>


using namespace std;

class FlatMemAllocatorRR : public  MemoryAllocator{
private:
	size_t maximumSize;
	size_t allocatedSize;
	vector<char> memory;
	unordered_map<size_t, bool> allocationMap;

	void initializeMemory();
	bool canAllocateAt(size_t index, size_t size);
	void allocateAt(size_t index, size_t size);
	void deallocateAt(size_t index);

public:
	FlatMemAllocatorRR(size_t maximumSize);
	FlatMemAllocatorRR() = default;

	void* allocate(size_t size) override;
	void* deallocate(void* ptr) override;
	string visualizeMemory() override;

};

#endif