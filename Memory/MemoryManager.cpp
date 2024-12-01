#include "MemoryManager.h"
#include "../TypeDefRepo.h"

MemoryManager* MemoryManager::instance = nullptr;

MemoryManager::MemoryManager(){
    this->max_overall_mem = GlobalConfig::getInstance()->getMaxOverallMem();
    this->mem_per_frame = GlobalConfig::getInstance()->getMemPerFrame();
    this->max_mem_per_proc = GlobalConfig::getInstance()->getMaxMemPerProcess();
    this->min_mem_per_proc = GlobalConfig::getInstance()->getMinMemPerProcess();
    this->pagedIn = 0;
    this->pagedOut = 0;
    setAllocator();

    // Create backing store directory if it doesn't exist
    if(system("mkdir -p backingstore") != 0) {
        std::cerr << "Error: Could not create backingstore directory" << std::endl;
    }
}

void MemoryManager::initialize()
{
	if (instance == nullptr) {
		instance = new MemoryManager();
	}
}

void MemoryManager::destroy() {
	delete instance;
}

void MemoryManager::setAllocator()
{
    this->numFrames = floor(max_overall_mem / mem_per_frame);
    if(max_overall_mem == mem_per_frame){
        memAllocator = std::make_unique<FlatMemoryAllocator>(max_overall_mem);
    } else {
        memAllocator = std::make_unique<PagingAllocator>(max_overall_mem);
    }
}

bool MemoryManager::isMemFull() const {
    return memAllocator->isMemFull();
}

void MemoryManager::allocateProcess(std::shared_ptr<Process> process)
{
    mtx.lock();
    size_t processBaseAddress = memAllocator->allocate(process);
    
    // remaining memory is the max memory - the memory already allocated
    size_t remainingMem = max_overall_mem - this->currentMemAllocated();
    mtx.unlock();
    
    // debugger
    // std::cout << "\nAttempting to allocate " << process->getName() 
    //           << " (size: " << process->getMemoryRequired() 
    //           << "/" << remainingMem << " available)" << std::endl;
              
    if(processBaseAddress != -1) {
        process->setMemoryStatus(true);
        process->setMemBaseAddress(processBaseAddress);
        // debugger
        // std::cout << "Allocation successful at address: " << processBaseAddress << std::endl;
    } else {
        if (memAllocator->isMemFull()) {
            doBackingStore(process);
        }
    }
}

void MemoryManager::deallocateProcess(std::shared_ptr<Process> process){
    size_t procBaseAddress = process->getMemBaseAddress();
    if(procBaseAddress != -1){
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
    if (memAllocator->isMemFull()) {
        // debugger
        std::cout << "Memory full, initiating backing store for " << process->getName() << std::endl;  // Debug log
        this->doBackingStore(process);
    }
    else {
        if (this->checkBackingStore(process)) {
            this->removeFromBackingStore(process);
        }
        if (!this->checkProcessList(process)) {
            mtx.lock();
            this->processList.push_back(process);
            mtx.unlock();
            this->allocateProcess(process);
        }
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

void MemoryManager::doBackingStore(std::shared_ptr<Process> process){
    std::shared_ptr<Process> lastProcess = processList.front();

    std::cout << "Paging out process: " << lastProcess->getName() << std::endl;  // Debug log

    storeToBackingStore(lastProcess);
    removeProcessFromMemory(lastProcess);
    allocateProcess(process);
}

void MemoryManager::storeToBackingStore(std::shared_ptr<Process> process){

    if(!checkBackingStore(process)) {
        mtx.lock();
        // Create backing store file
        String filename = "backingstore/" + process->getName() + ".txt";
        std::ofstream outFile(filename);
        if(outFile.is_open()) {
            // Write process state to file
            outFile << "PID: " << process->getPID() << std::endl;
            outFile << "Memory Required: " << process->getMemoryRequired() << std::endl;
            outFile << "Base Address: " << process->getMemBaseAddress() << std::endl;
            outFile << "Memory Status: " << process->getMemoryStatus() << std::endl;
            outFile << "Frames Required: " << process->getFramesRequired() << std::endl;
            outFile.close();
            
            removeProcessFromMemory(process);
            this->backingStore.push_back(process);
            this->pagedOut += process->getFramesRequired();
            std::cout << "Process " << process->getName() << " paged out to " << filename << std::endl;
        } else {
            std::cerr << "Error: Could not create backing store file for " << process->getName() << std::endl;
        }
        mtx.unlock();
    }
}

void MemoryManager::removeFromBackingStore(std::shared_ptr<Process> process){
    auto it = std::find(backingStore.begin(), backingStore.end(), process);
    if (it != backingStore.end()) {
        // Remove backing store file
        String filename = "backingstore/" + process->getName() + ".txt";
        if(remove(filename.c_str()) != 0) {
            std::cerr << "Error: Could not delete backing store file " << filename << std::endl;
        } else {
            this->pagedIn += process->getFramesRequired();
            std::cout << "Process " << process->getName() << " paged in from " << filename << std::endl;
        }
        backingStore.erase(it);
    }
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

std::vector<std::shared_ptr<Process>> MemoryManager::getProcessList(){
    return this->processList;
}

size_t MemoryManager::getPagedIn(){
    return this->pagedIn;
}

size_t MemoryManager::getPagedOut(){
    return this->pagedOut;    
}

size_t MemoryManager::getFreeMemory() const {
    return this->max_overall_mem - this->currentMemAllocated();
}