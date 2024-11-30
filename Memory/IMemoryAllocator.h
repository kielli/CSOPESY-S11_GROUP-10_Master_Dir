#pragma once

#include "../Process/Process.h"

class IMemoryAllocator {
public:
	virtual bool allocate(std::shared_ptr<Process> process) = 0;
    virtual void deallocate(std::shared_ptr<Process> process) = 0;
    //virtual void printMem() = 0;
    virtual void printProcesses() = 0;
    virtual void vmstat() = 0;
};