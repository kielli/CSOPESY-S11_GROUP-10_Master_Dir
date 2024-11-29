#pragma once

#include "IMemoryAllocator.h"

#include <unordered_map>
#include <vector>
#include <queue>

class PagingAllocator : public IMemoryAllocator 
{
	public:
		PagingAllocator(size_t maxMemorySize, size_t numFrames);

		void* allocate(Process* process) override;
	    void deallocate(Process* process) override;
	    void visualizeMemory() const override;
	
	private:
		size_t maxMemorySize;
		size_t numFrames;

		std::unordered_map<size_t, size_t> frameMap;
		std::queue<size_t> freeFrameList;
		
		size_t allocateFrames(size_t numFrames, size_t processId);
		void deallocateFrames(size_t frameIndex);
};