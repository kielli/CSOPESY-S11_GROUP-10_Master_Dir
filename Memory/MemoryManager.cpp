#include "MemoryManager.h"
#include <Scheduler.h>
#include "PagingAllocator.h"
#include "FlatMemoryAllocator.h"

MemoryManager* MemoryManager::sharedInstance = nullptr;

MemoryManager* MemoryManager::getInstance()
{
	return sharedInstance;
}

void MemoryManager::initialize()
{
	sharedInstance = new MemoryManager();
}

void MemoryManager::destroy()
{
	delete sharedInstance;
}

void MemoryManager::displayMemoryStatus()
{
	for (auto process : processListInMemory)
	{
		std::cout << "Total external fragmentation in KB: " << calculateExternalFragmentation() << std::endl;
		std::cout << "Process " << process->getName() << " at the base address " << (process->getBaseAddress() - this->baseAddress) << " with size " << process->getMemorySize() << std::endl;
		std::cout << " " << std::endl;
	}
}

void MemoryManager::displayBackingStore()
{
	std::cout << "Backing Store: \n";
    for (const auto& process : backingStore) {
        std::cout << "Process " << process.second->getName() << ".\n";
    }
}

void MemoryManager::exportMemoryStatusToFile(int quantum)
{
	std::ofstream file("memory_stamp_" + std::to_string(quantum) + ".txt");
    sortMemoryDisplay();
    if (file.is_open()) {
        file << "Timestamp: (" << updateTimeStamp() << ")\n";
        file << "Number of processes in memory: " << processListInMemory.size() << "\n";
        file << "Total external fragmentation in KB: " << calculateExternalFragmentation() << "\n";
        file << "\n----end---- = " << this->maxMemory << "\n";
        
        for (const auto& process : processListInMemory) {
            file << "\n" << (process->getBaseAddress() - this->baseAddress + process->getMemorySize() - 1) << "\n"
                << process->getName()
                << "\n" << (process->getBaseAddress() - this->baseAddress) << "\n";
        }
       
        file << "\n ----start---- = 0";
        file.close();
    }
    else {
        std::cerr << "Error: Could not open file to save memory state.\n";
    }
}

size_t MemoryManager::getUsedMemory() const
{
    size_t occupiedSpace = 0;
    for (const auto& process : processListInMemory) {
        if(process != nullptr)
            occupiedSpace += process->getMemorySize();
    }
    return occupiedSpace;
}

size_t MemoryManager::calculateExternalFragmentation() const
{
    return maxMemory - getUsedMemory();
}

int MemoryManager::getPagedInCount() const
{
    return this->pageInCount;
}

int MemoryManager::getPagedOutCount() const
{
    return this->pageOutCount;
}

bool MemoryManager::isMemoryFull() const
{
    return memoryAllocator->isFull();
}

void MemoryManager::addProcessToMemory(std::shared_ptr<Process> process)
{
    if (backingStoreContains(process)) {
		removePageFromBackingStore(process);
	}

	if (!containsPage(process)) {
		memoryMutex.lock();
        timeMap[processCount] = process;
        processCount++;
        memoryMutex.unlock();
        memoryMutex.lock();
        processListInMemory.push_back(process);
        memoryMutex.unlock();
        allocateMemory(process);
	}
}

void MemoryManager::removeProcessFromMemory(std::shared_ptr<Process> process)
{
	auto it = std::remove(processListInMemory.begin(), processListInMemory.end(), process);
    if(it != processListInMemory.end()) {
        deallocateMemory(process);
        memoryMutex.lock();
        processListInMemory.pop_back();
        memoryMutex.unlock();
    }
}

bool MemoryManager::backingStoreContains(std::shared_ptr<Process> process) const
{
    return backingStore.find(process->getPID()) != backingStore.end();
}

void MemoryManager::addPageToBackingStore(std::shared_ptr<Process> process)
{
	if (!backingStoreContains(process)) {
		this->backingStore[process->getPID()] = process;
		this->writeToFile();
		this->pageInCount += process->getNumberOfPages();
	}
}

void MemoryManager::removePageFromBackingStore(std::shared_ptr<Process> process)
{
	int pid = process->getPID();
	auto it = std::find_if(timeMap.begin(), timeMap.end(), [pid](const std::pair<int, std::shared_ptr<Process>>& entry) {
        return entry.second->getPID() == pid;
    });

	if (it != timeMap.end()) {
		timeMap.erase(it);
	}

	backingStore.erase(pid);
	this->pageOutCount += process->getNumberOfPages();

	this->writeToFile();

}

void MemoryManager::executeBackingStoreOperation(std::shared_ptr<Process> process)
{
	auto oldest = timeMap.begin()->second;
    
    memoryMutex.lock();
    timeMap.erase(timeMap.begin());
    memoryMutex.unlock();

    addPageToBackingStore(oldest);
    removeProcessFromMemory(oldest);
    allocateMemory(process);
}

bool MemoryManager::containsPage(std::shared_ptr<Process> process) const
{
    auto it = std::find(processListInMemory.begin(), processListInMemory.end(), process);
    if (it != processListInMemory.end()) {
        return true;
    }
    return false;
}

size_t MemoryManager::getMaxMemory() const
{
    return this->maxMemory;
}

void MemoryManager::setMaxMemory(size_t maxMemory)
{
	this->maxMemory = maxMemory;
}

void MemoryManager::setMemoryPerFrame(size_t memoryPerFrame)
{
	this->memoryPerFrame = memoryPerFrame;
}

void MemoryManager::setMinMemoryPerProcess(size_t minMemoryPerProcess)
{
	this->minMemoryPerProcess = minMemoryPerProcess;
}

void MemoryManager::setMaxMemoryPerProcess(size_t maxMemoryPerProcess)
{
	this->maxMemoryPerProcess = maxMemoryPerProcess;
}

void MemoryManager::setMemoryAllocator()
{
	this->frameCount = static_cast<size_t>(std::floor(this->maxMemory/ this->memoryPerFrame));
    if (this->maxMemory == this->memoryPerFrame) {
        memoryAllocator = std::make_unique<FlatMemoryAllocator>(baseAddress, maxMemory);
    }
    else {
        memoryAllocator = std::make_unique<PagingAllocator>(maxMemory, frameCount);
    }
}

size_t MemoryManager::getMemoryPerFrame() const
{
    return this->memoryPerFrame;
}

// Constructor
MemoryManager::MemoryManager():
	baseAddress(0),
	minMemoryPerProcess(0),
	maxMemoryPerProcess(0),
	frameCount(0),
	pageInCount(0),
	pageOutCount(0),
	processCount(0)
{
	this->maxMemory = GlobalConfig::getInstance()->getMaxOverallMem();
	this->memoryPerFrame = GlobalConfig::getInstance()->getMemPerFrame();
}

String MemoryManager::updateTimeStamp()
{
    time_t currentTime = time(0);
    char buffer[100];
    struct tm timeInfo;
    localtime_s(&timeInfo, &currentTime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeInfo);
    return buffer; 
}

void MemoryManager::allocateMemory(std::shared_ptr<Process> process)
{
	memoryMutex.lock();
    char* allocatedMemory = static_cast<char*>(memoryAllocator->allocate(process));
    memoryMutex.unlock();
    if (allocatedMemory != nullptr) {
        memoryAllocator->allocate(process);
        //printMemoryState();
        //std::cerr << "Successfully allocated memory for Process " << process->getPID() << ".\n";
    }
    else {
        //std::cerr << "Error: Failed to allocate memory for Process " << process->getPID() << ".\n";
        executeBackingStoreOperation(process);
    }
}

void MemoryManager::deallocateMemory(std::shared_ptr<Process> process)
{
	void* baseAddress = process->getBaseAddress();

	if (baseAddress != nullptr) {
		memoryAllocator->deallocate(process);
	}

}

void MemoryManager::sortMemoryDisplay()
{
	bool sorted = false;
    while (!sorted) {
        sorted = true;
        for (int i = 0; i < processListInMemory.size() - 1; i++) {
            if (processListInMemory[i]->getBaseAddress() < processListInMemory[i + 1]->getBaseAddress()) {
                auto temp = processListInMemory[i];
                processListInMemory[i] = processListInMemory[i + 1];
                processListInMemory[i + 1] = temp;
                sorted = false;
            }
        }
    }
}

void MemoryManager::readFromFile()
{
	this->backingStore.clear();

    int processId, size, counter;
    std::string processName, inputLine;

    std::ifstream backingStore("BackingStore.txt");
    backingStore >> size;

    for (int i = 0; i < size; i++) {
        backingStore >> processId;
        backingStore >> processName;
        backingStore >> counter;
        std::getline(backingStore, inputLine);
        auto process = Scheduler::getInstance()->createUniqueProcess(processName);
        this->backingStore[processId] = process;
    }

    backingStore.close();
}

void MemoryManager::writeToFile()
{
	std::ofstream backingStore("BackingStore.txt");
    backingStore << this->backingStore.size() << "\n";
    for (const auto& process : this->backingStore) {
        if (process.second != nullptr) {
            backingStore << process.second->getPID() << " "
                << process.second->getName() << " "
                << process.second->getCommandCounter() << "\n\tMemory Size: "
                << process.second->getMemorySize() << " Pages: "
                << process.second->getNumberOfPages() << "\n";
        }
    }

    backingStore.close();
}
