#pragma once

#include "../TypeDefRepo.h"
#include "../Process/Process.h"

class IMemoryAllocator {
	public:
		virtual void* allocate(Process* process) = 0;
		virtual void deallocate(Process* process) = 0;
		virtual void visualizeMemory() const = 0;
};