#pragma once

#include <unordered_map>

#include "IMemoryAllocator.h"
#include "../Process/Process.h"

class FlatMemoryAllocator : public IMemoryAllocator {
	public:
		FlatMemoryAllocator(size_t maximumSize);
		~FlatMemoryAllocator();

		void* allocate(Process* process) override;
		void deallocate(Process* process) override;
		void visualizeMemory() const = 0;

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

