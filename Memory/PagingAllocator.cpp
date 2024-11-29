#include "PagingAllocator.h"
#include "../Process/Process.h"

// Implementation of PagingAllocator Class
PagingAllocator::PagingAllocator(size_t maxMemorySize, size_t numFrames)
	: maxMemorySize(maxMemorySize), numFrames(maxMemorySize)
{
	// Initialize the free frame list
	for(size_t i = 0; i < numFrames; i++) {
		freeFrameList.push(i);
	}
}

void* PagingAllocator::allocate(Process* process)
{
	size_t processId = process->getPID();
	size_t numFramesNeeded = process->getNumPages();
	
	if(numFramesNeeded > freeFrameList.size()) {
		std::cerr << "Memory allocation failed. Not enough free frames.\n";
		return nullptr;
	}
	
	// Allocate frames for the process
	size_t frameIndex = allocateFrames(numFramesNeeded, processId);
	return reinterpret_cast<void*>(frameIndex);
}

void PagingAllocator::deallocate(Process* process)
{
	size_t processId = process->getPID();
    std::vector<size_t> deallocateAFrame;

	// Find frames allocated to the process and deallocate
	for(const auto& entry : frameMap) {
        if(entry.second == processId) {
            deallocateAFrame.push_back(entry.first);
        }
    }

    for (size_t frameIndex : deallocateAFrame) {
        deallocateFrames(frameIndex);
    }
}

void PagingAllocator::visualizeMemory() const
{
	std::cout << "Memory Visualization:\n";
	
	for(size_t frameIndex = 0; frameIndex < numFrames; ++frameIndex) {
		auto it = frameMap.find(frameIndex);
		
		if(it != frameMap.end()) {
			std::cout << "Frame " << frameIndex << "  -> Process " << it->second << "\n";
		}
		else {
			std::cout << "Frane " << frameIndex << " -> Free\n";
		}
	}
	
	std::cout << "--------------------\n";
}

size_t PagingAllocator::allocateFrames(size_t numFrames, size_t processId)
{
	size_t frameIndex = freeFrameList.back();
	freeFrameList.pop();
	
	// Map allocated frames to the process ID
	for (size_t i = 0; i < numFrames; i++) {
		frameMap[frameIndex + i] = processId;
	}
	
	return frameIndex;
}

void PagingAllocator::deallocateFrames(size_t frameIndex)
{
	// Remove mapping of deallocate frames
	for (size_t i = 0; i < numFrames; i++) {
		frameMap.erase(frameIndex + i);
	}
	
	// Add frames back to the free frame list
	for (size_t i = 0; i < numFrames; ++i) {
		freeFrameList.push(frameIndex + i);
	}
}

// void PagingScheduler::addProcess(Process* process)
// {
// 	// Allocate memory for the process
// 	void* memoryBlock = memoryAllocator.allocate(process);
	
// 	if(!memoryBlock)
// 	{
// 		std::cerr << "Failed to add process " << process->getId() << ". Memory allocation failed.\n";
// 		return;
// 	}
	
// 	size_t coreId = rand() % numCores; // Assign the process to a random core
	
// 	processCores[process] = coreId;
// 	process->setMemoryBlock(memoryBlock);
// 	std::cout << "Process " << process->getId() << " added to Core " << coreId << ".\n";
// }

// void PagingScheduler::removeProcess(Process* process)
// {
// 	// Deallocate memory for the process
// 	memoryAllocator.deallocate(process);
// 	processCores.erase(process);
// 	std::cout << "Process " << process->getId() << " removed.\n";
// }

// void PagingScheduler::visualizeMemory() const {
// 	memoryAllocator.visualizeMemory();
// }