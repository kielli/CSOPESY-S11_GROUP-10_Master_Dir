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
class FlatMemoryAllocator: public MemoryAllocator {
private:
	size_t maximumSize;
	size_t allocatedSize;
	vector<char> memory;
	unordered_map<size_t, bool> allocationMap;

	void initializeMemory() {
		fill(memory.begin(), memory.end(), '.');
		//fill(allocationMap.begin(), allocationMap.end(), false);
		for (size_t i = 0; i < maximumSize; ++i) {
			allocationMap[i] = true;
		}
	}
	bool canAllocateAt(size_t index, size_t size) const {
		return (index + size <= maximumSize);
	}
	void allocateAt(size_t index, size_t size) {
		for (size_t i = index; i < index + size; ++i) {
			allocationMap[i] = true;
		}
		allocatedSize += size;
	}
	void deallocateAt(size_t index) {
		allocationMap[index] = false;
	}
public:
	FlatMemoryAllocator(size_t maximumSize);
	~FlatMemoryAllocator();
	void* allocate(size_t size) override;
	void deallocate(void* ptr) override;
	string visualizeMemory() override;
};
#endif