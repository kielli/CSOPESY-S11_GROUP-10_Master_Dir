#include "PagingAllocator.h"
#include <mutex>
PagingAllocator::PagingAllocator(size_t maxMemSize) : maxMemSize(maxMemSize), numFrames(maxMemSize) {
    for(size_t i = 0; i < numFrames; i++){
        freeFrameList.push_back(i);
    }
}

void* PagingAllocator::allocate(Process* process){
    size_t processId = process->getPID();
    size_t numFramesNeeded = process->getFramesRequired();
    if(numFramesNeeded > freeFrameList.size())
        return nullptr;
    size_t frameIndex = allocateFrames(numFramesNeeded, processId);
    return reinterpret_cast<void*>(frameIndex);
}

void PagingAllocator::deallocate(Process* process){
    size_t processId = process->getPID();
    std::vector<size_t> framesToBeRemoved;

    for(const auto& entry : frameMap){
        if(entry.second == processId)
            framesToBeRemoved.push_back(entry.first);
    }
    deallocateFrames(framesToBeRemoved);
}

void PagingAllocator::deallocateFrames(std::vector<size_t> framesToBeFreed) {
    mtx.lock();
    for(size_t frame : framesToBeFreed){
        frameMap.erase(frame);
    }
    for(size_t frame : framesToBeFreed){
        freeFrameList.push_back(frame);
    }
    mtx.unlock();
}

size_t PagingAllocator::allocateFrames(size_t numFrames, size_t processId){
    size_t frameIndex = freeFrameList.back();
    freeFrameList.pop_back();
	mtx.lock();
	for(size_t i = 0; i < numFrames; ++i){
        frameMap[frameIndex+i] = processId;
    }
	mtx.unlock();
	return frameIndex;
}

bool PagingAllocator::isMemFull() const {
	return freeFrameList.empty();
}