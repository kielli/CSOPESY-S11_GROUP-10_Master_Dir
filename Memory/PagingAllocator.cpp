#include "PagingAllocator.h"

PagingAllocator::PagingAllocator(size_t maxMemorySize, size_t numFrames) 
	: maxMemorySize(maxMemorySize), numFrames(numFrames) {
	for (size_t i = 0; i < numFrames; i++) {
		freeFrameList.push(i);
	}
}

void* PagingAllocator::allocate(std::shared_ptr<Process> process) {
	size_t processId = process->getPID();
	size_t numFrames = process->getNumberOfPages();

	if (numFrames > freeFrameList.size()) {
		//std::cerr << "Memory allocation failed. Not enough free frames.\n";
		return nullptr;
	}

	size_t frameIndex = allocateFrames(numFrames, processId);
	return reinterpret_cast<void*>(frameIndex);
}

void PagingAllocator::deallocate(std::shared_ptr<Process> process) {
	size_t processId = process->getPID();
	std::vector<size_t> framesToDeallocate;

	for (const auto& entry : frameMap) {
		if (entry.second == processId) {
			framesToDeallocate.push_back(entry.first);
		}
	}
	
	for (size_t frameIndex : framesToDeallocate) {
		deallocateFrames(frameIndex);
	}
}

void PagingAllocator::visualizeMemory() const {
	std::cout << "Memory Visualization:\n";
	for (size_t frameIndex = 0; frameIndex < numFrames; ++frameIndex) {
		auto it = frameMap.find(frameIndex);
		if (it != frameMap.end()) {
			std::cout << "Frame " << frameIndex << " -> Process " << it->second << "\n";
		}
		else {
			std::cout << "Frame " << frameIndex << " -> Free\n";
		}
	}
	std::cout << "----------------------------\n";
}

bool PagingAllocator::isFull() const {
	return freeFrameList.empty();
}

size_t PagingAllocator::allocateFrames(size_t numFrames, size_t processId) {
	size_t frameIndex = freeFrameList.front();
	mtx.lock();
	for (size_t i = 0; i < numFrames; i++) {
		auto index = freeFrameList.front();
		freeFrameList.pop();
		frameMap[index] = processId;
	}
	mtx.unlock();
	return frameIndex;
}

void PagingAllocator::deallocateFrames(size_t frameIndex) {
	mtx.lock();
	//Remove mapping of deallocated frame
	frameMap.erase(frameIndex);

	//Add frames back to the free frame list
	freeFrameList.push(frameIndex);
	mtx.unlock();
}