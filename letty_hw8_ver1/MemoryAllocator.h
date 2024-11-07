#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <string>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

class MemoryAllocator {
protected:


public:
	virtual ~MemoryAllocator() {};
	virtual void* allocate(size_t size) = 0;
	virtual void* deallocate(void* ptr) = 0;
	virtual string visualizeMemory() = 0;
};

#endif