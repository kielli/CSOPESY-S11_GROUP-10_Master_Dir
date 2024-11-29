#pragma once
#include <algorithm>
#include <string>
#include <iostream>

#include "IMemoryAllocator.h"
#include "../Process/Process.h"


class FlatMemoryAllocator : public IMemoryAllocator
{
public:
	FlatMemoryAllocator(char* baseAddress, size_t maximumSize);
	~FlatMemoryAllocator();

	void* allocate(std::shared_ptr<Process> process) override;
	void deallocate(std::shared_ptr<Process> process) override;
	void visualizeMemory() const override;

	bool isFull() const override;

private:
	size_t maximumSize;
	size_t allocatedSize;

	char* baseAddress;
	std::vector<bool> allocationMap;

	bool canAllocateAt(size_t index, size_t size) const;
	void allocateAt(size_t index, size_t size);
	void deallocateAt(size_t index, size_t size);
};

