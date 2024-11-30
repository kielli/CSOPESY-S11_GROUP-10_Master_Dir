#pragma once

#include "IMemoryAllocator.h"

using namespace std;

#include <queue>
#include <unordered_map>

class PagingAllocator : public IMemoryAllocator {
    public:
        PagingAllocator(int numFrames);

        struct Frame {
            int frameId;
            bool isFrameFree;
        };

        void* allocate(Process* process) override;
        void deallocate(Process* process) override;
        void visualizeMemory() override;

    private:
        int numFrames; 

        queue<Frame> freeFrameList; // list of free frames
        unordered_map<size_t, size_t> pageTable; // page table: page num, frame num 
};