#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H
#include <string>
class MemoryAllocator {
public:
	virtual void* allocate(size_t size) = 0;
	virtual void deallocate(void* ptr) = 0;
	virtual std::string visualizeMemory() = 0;
};
#endif