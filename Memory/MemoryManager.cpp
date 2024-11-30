#include "MemoryManager.h"

MemoryManager* MemoryManager::instance = nullptr;

MemoryManager* MemoryManager::getInstance() {
	return instance;
}

void MemoryManager::initialize() {
	instance = new MemoryManager();
}

void MemoryManager::destroy() {
	delete instance;
}

