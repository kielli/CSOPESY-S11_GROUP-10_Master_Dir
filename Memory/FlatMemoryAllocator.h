#pragma once
#include <unordered_map>

#include "IMemoryAllocator.h"


class FlatMemoryAllocator : public IMemoryAllocator
{
public:
	FlatMemoryAllocator(size_t maximumSize);
	~FlatMemoryAllocator();

	void* allocate(size_t size) override;
	void deallocate(void* ptr) override;

	String visualizeMemory() override;

private:
	size_t maximumSize;
	size_t allocatedSize;
	CharVector memory;
	std::unordered_map<size_t, bool> allocationMap;

	void initializeMemory();
	bool canAllocateAt(size_t index, size_t size) const;
	void allocateAt(size_t index, size_t size);
	void deallocateAt(size_t index);
};

