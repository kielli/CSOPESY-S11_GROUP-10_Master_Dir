#pragma once

#include "../Process/Process.h"

class IMemoryAllocator {
    public:
        virtual size_t allocate(std::shared_ptr<Process> process) = 0;
        virtual void deallocate(std::shared_ptr<Process> process) = 0;
        virtual bool isMemFull() const = 0;
};