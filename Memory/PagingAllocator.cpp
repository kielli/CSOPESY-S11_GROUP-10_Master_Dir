#include "PagingAllocator.h"
#include <mutex>

PagingAllocator::PagingAllocator(size_t maxMemSize) : maxMemSize(maxMemSize) {
    size_t frameSize = GlobalConfig::getInstance()->getMemPerFrame();
    numFrames = maxMemSize / frameSize;
    
    // Initialize free frame list
    for(size_t i = 0; i < numFrames; i++) {
        freeFrameList.push_back(i);
    }
}

size_t PagingAllocator::allocate(std::shared_ptr<Process> process) {
    size_t frameSize = GlobalConfig::getInstance()->getMemPerFrame();
    size_t framesNeeded = ceil(static_cast<double>(process->getMemoryRequired()) / frameSize);
    
    if(framesNeeded > freeFrameList.size()) {
        return -1;
    }

    std::lock_guard<std::mutex> lock(mtx);
    size_t baseFrame = allocateFrames(framesNeeded, process->getPID());
    return (baseFrame != -1) ? baseFrame * frameSize : -1;
}

void PagingAllocator::deallocate(std::shared_ptr<Process> process){
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

size_t PagingAllocator::allocateFrames(size_t framesNeeded, size_t processId) {
    if(freeFrameList.empty() || framesNeeded > freeFrameList.size()) {
        return -1;
    }

    size_t baseFrame = freeFrameList.front();
    for(size_t i = 0; i < framesNeeded; i++) {
        frameMap[freeFrameList.front()] = processId;
        freeFrameList.erase(freeFrameList.begin());
    }
    return baseFrame;
}

bool PagingAllocator::isMemFull() const {
	return freeFrameList.empty();
}