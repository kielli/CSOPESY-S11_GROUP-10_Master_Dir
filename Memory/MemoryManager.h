#pragma once
#include "FlatMemoryAllocator.h"
#include "PagingAllocator.h"
#include "Scheduler.h"
#include "Process.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <map>

class MemoryManager {
    public:
        void readBackingStoreFile();
        void writeBackingStoreFile();
};