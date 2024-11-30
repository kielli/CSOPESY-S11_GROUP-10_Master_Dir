#include "MemoryManager.h"

MemoryManager::MemoryManager(){
    this->max_overall_mem = GlobalConfig::getInstance()->getMaxOverallMem();
    this->mem_per_frame = GlobalConfig::getInstance()->getMemPerFrame();
    this->max_mem_per_proc = GlobalConfig::getInstance()->getMaxMemPerProcess();
    this->min_mem_per_proc = GlobalConfig::getInstance()->getMinMemPerProcess();
    this->pagedIn = 0;
    this->pagedOut = 0;
}

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

void MemoryManager::initializeProcessMem(std::shared_ptr<Process> process) {
    size_t randomNum = min_mem_per_proc + (rand() % (max_mem_per_proc - min_mem_per_proc + 1));
    process->initializeMemory(randomNum);
}

void MemoryManager::setAllocator(){
    this->numFrames = floor(max_overall_mem / mem_per_frame);
    if(max_overall_mem == mem_per_frame){
        memAllocator = std::make_unique<FlatMemoryAllocator>(max_overall_mem);
    } else {
        memAllocator = std::make_unique<PagingAllocator>(max_overall_mem);
    }
}

void MemoryManager::allocateProcess(std::shared_ptr<Process> process){
    mtx.lock();
    char* processBaseAddress = static_cast<char*>(memAllocator->allocate(process));
    mtx.unlock();
    if(processBaseAddress != nullptr){
        process->setMemoryStatus(true);
    } else {

    }
    
}

void MemoryManager::deallocateProcess(std::shared_ptr<Process> process){
    char* procBaseAddress = process->getMemBaseAddress();
    if(procBaseAddress != nullptr){
        mtx.lock();
        memAllocator->deallocate(process);
        process->setMemoryStatus(false);
        mtx.unlock();
    }
}

size_t MemoryManager::currentMemAllocated() const{
    size_t currentMemAllocated = 0;
    for(std::shared_ptr<Process> process : processList){
        if(process->getMemoryStatus()){
            currentMemAllocated += process->getMemoryRequired();
        }
    }
    return currentMemAllocated;
}

void MemoryManager::addProcessToMemory(std::shared_ptr<Process> process){
    if(checkBackingStore(process)){
        removeFromBackingStore(process);
    }
    if(!checkProcessList(process)){
        mtx.lock();
        processList.push_back(process);
        mtx.unlock();
        allocateProcess(process);
    }
}

void MemoryManager::removeProcessFromMemory(std::shared_ptr<Process> process){
    auto it = std::remove(processList.begin(), processList.end(), process);
    if(it != processList.end()) {
        deallocateProcess(process);
        mtx.lock();
        processList.pop_back();
        mtx.unlock();
    }
}

void MemoryManager::storeToBackingStore(std::shared_ptr<Process> process){
    if(!checkBackingStore(process)){
        mtx.lock();
        this->backingStore.push_back(process);
        this->pagedIn += process->getFramesRequired();
        mtx.unlock();
    }
}

void MemoryManager::removeFromBackingStore(std::shared_ptr<Process> process){
    auto it = std::find(backingStore.begin(), backingStore.end(), process);
    if (it != backingStore.end())
        backingStore.erase(it);
}

bool MemoryManager::checkBackingStore(std::shared_ptr<Process> process){
    auto it = std::find(backingStore.begin(), backingStore.end(), process);
    if (it != backingStore.end()) {
        return true;
    }
    return false;
}

bool MemoryManager::checkProcessList(std::shared_ptr<Process> process){
    auto it = std::find(processList.begin(), processList.end(), process);
    if (it != processList.end()) {
        return true;
    }
    return false;
}









