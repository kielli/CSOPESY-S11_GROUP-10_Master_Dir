#pragma once

#include "../Process/Process.h"

class IMemoryAllocator {
    public:
        virtual void* allocate(std::shared_ptr<Process> process) = 0;
        virtual void deallocate(std::shared_ptr<Process> process) = 0;
};