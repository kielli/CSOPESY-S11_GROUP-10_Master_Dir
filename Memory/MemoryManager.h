#pragma once

#include "../Process/Process.h"
#include "IMemoryAllocator.h"
#include "FlatMemoryAllocator.h"
#include "PagingAllocator.h"
#include <mutex>
#include <cmath>
#include <memory>

class MemoryManager{
    public:
        MemoryManager();
        static MemoryManager* getInstance();
		static void initialize();
		static void destroy();

    private:
        static MemoryManager* instance;

        size_t max_overall_mem;
        size_t mem_per_frame;
        size_t min_mem_per_proc;
        size_t max_mem_per_proc;
        size_t numFrames;
        size_t pagedIn;
        size_t pagedOut;

        std::mutex mtx;
        std::unique_ptr<IMemoryAllocator> memAllocator;
        std::vector<std::shared_ptr<Process>> processList;
        std::vector<std::shared_ptr<Process>> backingStore;

        size_t initializeProcessMem();
        void initializeProcessMem(std::shared_ptr<Process> process);
        void setAllocator();
        void allocateProcess(std::shared_ptr<Process> process);
        void deallocateProcess(std::shared_ptr<Process> process);
        bool isMemFull() const;
        size_t currentMemAllocated() const;
        void addProcessToMemory(std::shared_ptr<Process> process);
        void removeProcessFromMemory(std::shared_ptr<Process> process);
        void storeToBackingStore(std::shared_ptr<Process> process);
        void removeFromBackingStore(std::shared_ptr<Process> process);
        bool checkBackingStore(std::shared_ptr<Process> process);
        bool checkProcessList(std::shared_ptr<Process> process);
};