#include "FlatMemAllocatorRR.h"

void FlatMemAllocatorRR::initializeMemory()
{
	fill(memory.begin(), memory.end(), '.');  //unallocated memory
	fill(allocationMap.begin(), allocationMap.end(), false); //all memory stolt not allocated
}

bool FlatMemAllocatorRR::canAllocateAt(size_t index, size_t size)
{
	return (index + size <= maximumSize);
}

void FlatMemAllocatorRR::allocateAt(size_t index, size_t size)
{
		for (size_t i = index; i < index + size; ++i) {
			allocationMap[i] = true;
		}

	/*fill(allocationMap.begin() + index, allocationMap.begin() + index + size, true);*/
}

void FlatMemAllocatorRR::deallocateAt(size_t index)
{
	allocationMap[index] = false;
}

FlatMemAllocatorRR::FlatMemAllocatorRR(size_t maximumSize) : maximumSize(maximumSize), allocatedSize(0)
{
	memory.resize(maximumSize, '.');
	/*memory.reserve(maximumSize);
	allocationMap.resize(maximumSize, false);*/
	initializeMemory();

}

//FlatMemAllocatorRR::FlatMemAllocatorRR()
//{
//	memory.clear();
//}

void* FlatMemAllocatorRR::allocate(size_t size)
{
	//find first available block that can accommodate the process
	for (size_t i = 0; i < maximumSize - size + 1; ++i) {
		if (!allocationMap[i] && canAllocateAt(i, size)){
			allocateAt(i, size);
			return &memory[i];
		}
	}
}

void* FlatMemAllocatorRR::deallocate(void* ptr)
{
	size_t index = static_cast<char*>(ptr) - &memory[0];
	deallocateAt(index);
}

string FlatMemAllocatorRR::visualizeMemory()
{
	return string(memory.begin(), memory.end());
}
