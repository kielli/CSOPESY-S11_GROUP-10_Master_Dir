#pragma once
#include <unordered_map>
#include <queue>
#include <mutex>


#include "IMemoryAllocator.h"
#include "../Process/Process.h"

class PagingAllocator : public IMemoryAllocator {
public:
	PagingAllocator(size_t maxMemorySize, size_t numFrames);

	void* allocate(std::shared_ptr<Process> process) override;
	void deallocate(std::shared_ptr<Process> process) override;
	void visualizeMemory() const override;

	bool isFull() const override;
private:
	size_t maxMemorySize;
	size_t numFrames;
	std::unordered_map<size_t, size_t> frameMap;
	std::queue<size_t> freeFrameList;
	std::mutex mtx;

	size_t allocateFrames(size_t numFrames, size_t processId);
	void deallocateFrames(size_t frameIndex);
};