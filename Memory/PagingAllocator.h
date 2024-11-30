#pragma once
#include "IMemoryAllocator.h"
#include <unordered_map>
#include <vector>
#include <mutex>

class PagingAllocator : public IMemoryAllocator {
    public:
        PagingAllocator(size_t maxMemSize);

        void* allocate(Process* process) override;
        void deallocate(Process* process) override;
		bool isMemFull() const;

    private:
        size_t maxMemSize;
        size_t numFrames;
        std::unordered_map<size_t, size_t> frameMap;
        std::vector<size_t> freeFrameList;
        std::mutex mtx;

        size_t allocateFrames(size_t numFrames, size_t proccessId);
        void deallocateFrames(std::vector<size_t> framesToBeFreed);
        
};