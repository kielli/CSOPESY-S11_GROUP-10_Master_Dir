#pragma once

#include "../TypeDefRepo.h"

class IMemoryAllocator
{
public:
	virtual void* allocate(std::shared_ptr<Process> process) = 0;
	virtual void deallocate(std::shared_ptr<Process> process) = 0;
	virtual void visualizeMemory() const = 0;

	virtual bool isFull() const = 0;
};