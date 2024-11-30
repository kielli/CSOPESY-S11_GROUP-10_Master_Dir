#include "FlatMemoryAllocator.h"
#include <algorithm>

using namespace std;

FlatMemoryAllocator::FlatMemoryAllocator(int maxOverAllMemory) : maxOverAllMemory(maxOverAllMemory)
{
    std::ofstream backingStoreFile(".pagefile", std::ios::trunc);
    backingStoreFile.close();
}

bool FlatMemoryAllocator::allocate(std::shared_ptr<Process> process)
{
    int requiredMemory = process->getMemoryRequired();

    this->readBackingStore(process);

    if (this->memory.size() == 0) {
        this->memory.push_back(std::make_pair(process, std::make_pair(0, requiredMemory)));
        return true;
    }
    else {
        for (size_t i = 0; i < this->memory.size(); i++) {
            if (this->memory.at(i).first->getName() == process->getName()) {
                return true;
            }
        }

        if (this->memory.at(0).second.first >= requiredMemory) {
            this->memory.insert(this->memory.begin(), std::make_pair(process, std::make_pair(0, requiredMemory)));
            return true;
        }

        for (int retryLimit = 0; retryLimit < 5; retryLimit++) {
            int counter = 0;
            int index = -1;

            for (size_t i = 0; i < this->memory.size(); i++) {
                if (i == this->memory.size() - 1) {
                    counter = this->maxOverAllMemory - this->memory.at(i).second.second;
                }
                else {
                    counter += this->memory.at(i + 1).second.first - this->memory.at(i).second.second;
                }

                if (counter >= requiredMemory) {
                    this->memory.insert(this->memory.begin() + i + 1, std::make_pair(process, std::make_pair(this->memory.at(i).second.second, this->memory.at(i).second.second + requiredMemory)));
                    return true;
                }
            }

            if (this->memory.size() > 0) {
                int randomIndex = rand() % this->memory.size();
                if (this->memory.at(randomIndex).first->getCPUCoreID() == -1) {
                    this->writeBackingStore(this->memory.at(randomIndex).first);
                    this->memory.erase(this->memory.begin() + randomIndex);
                }
            }
        }
    }

    return false;
}

void FlatMemoryAllocator::deallocate(std::shared_ptr<Process> process) {
    if (!process) {
        std::cerr << "Error: Null process passed to deallocate()." << std::endl;
        return;
    }

    this->readBackingStore(process);

    for (size_t i = 0; i < this->memory.size(); i++) {
        if (this->memory.at(i).first->getName() == process->getName()) {
            this->memory.erase(this->memory.begin() + i);
            return;
        }
    }
}


void FlatMemoryAllocator::printProcesses()
{
    int counter = this->memory.size();
    int totalExternalFragmentation = 0;

    std::string lastProcessName = "";
    String output = "-----start----- = 0";
    int total = 0;

    StringVector processes;

    for (size_t i = 0; i < this->memory.size(); i++) {
        processes.push_back(this->memory.at(i).first->getName() + " " + std::to_string(this->memory.at(i).second.second - this->memory.at(i).second.first));
        total += this->memory.at(i).second.second - this->memory.at(i).second.first;
    }

    std::cout << "Memory Usage: " << total << " / " << this->maxOverAllMemory << std::endl;

    std::cout << "Memory Util: " << ((double)total / (double)this->maxOverAllMemory) * 100 << "%" << std::endl;

    std::cout << " " << std::endl;

    cout << "===============================================================" << endl;
	
    std::cout << "Running processes and memory usage: " << std::endl;

	cout << "---------------------------------------------------------------" << endl;

    for (size_t i = 0; i < processes.size(); i++) {
        std::cout << processes.at(i) << std::endl;
    }

    std::cout << "===============================================================" << std::endl;

    std::cout << std::endl;
}

void FlatMemoryAllocator::vmstat()
{
    int usedMemory = 0;

    if (this->memory.size() > 0) {
        usedMemory = this->memory[this->memory.size() - 1].second.second - this->memory[this->memory.size() - 1].second.first;
    }

    int active = 0;

    for (size_t i = 0; i < this->memory.size(); i++) {
        active += this->memory.at(i).second.second - this->memory.at(i).second.first;
    }

    int totalTicks = Scheduler::getInstance()->getTotalCPUTicks();
    int inactiveTicks = Scheduler::getInstance()->getInactiveCPUTicks();
    int activeTicks = totalTicks - inactiveTicks;

    usedMemory = active * 1024;
    int freeMemory = this->maxOverAllMemory - usedMemory;

    int pagedIn = 0;
    int pagedOut = 0;

    cout << "\nVMSTAT: " << endl;
	cout << maxOverAllMemory << "\t K Total Memory" << endl;
	cout << usedMemory << "\t K Used Memory" << endl;
	cout << freeMemory << "\t K Free Memory" << endl;
	cout << inactiveTicks << "\t Idle CPU ticks" << endl;
	cout << activeTicks << "\t Active CPU ticks" << endl;
	cout << totalTicks << "\t Total CPU ticks" << endl;
	cout << pagedIn << "\t Num Paged In" << endl;
	cout << pagedOut << "\t Num Paged Out" << endl;
}

void FlatMemoryAllocator::readBackingStore(std::shared_ptr<Process> process)
{
    std::ifstream backingStoreFile(".pagefile");

    if (!backingStoreFile.is_open()) {
        std::cerr << "Error: Could not open backing store file." << std::endl;
        return;
    }

    std::unordered_map<int, std::pair<int, int>> backingStore;
    String line, key, counter, memory;

    while (std::getline(backingStoreFile, line)) {
        std::istringstream(line) >> key >> counter >> memory;
        backingStore[std::stoi(key)] = std::make_pair(std::stoi(counter), std::stoi(memory));
    }

    backingStoreFile.close();

    if (backingStore.find(process->getPID()) != backingStore.end()) {
        backingStore.erase(process->getPID());
    }

    std::ofstream saveBackingStoreFile(".pagefile", std::ios::trunc);

    if (saveBackingStoreFile.is_open()) {
        for (const auto& entry : backingStore) {
            saveBackingStoreFile << entry.first << " " << entry.second.first << " " << entry.second.second << std::endl;
        }

        saveBackingStoreFile.close();
    }
}

void FlatMemoryAllocator::writeBackingStore(std::shared_ptr<Process> process)
{
    std::ifstream backingStoreFile(".pagefile");

    if (!backingStoreFile.is_open()) {
        std::cerr << "Error: Could not open backing store file." << std::endl;
        return;
    }

    std::unordered_map<int, std::pair<int, int>> backingStore;
    String line, key, counter, memory;

    while (std::getline(backingStoreFile, line)) {
        std::istringstream(line) >> key >> counter >> memory;
        backingStore[std::stoi(key)] = std::make_pair(std::stoi(counter), std::stoi(memory));
    }

    backingStoreFile.close();

    if (backingStore.find(process->getPID()) != backingStore.end()) {
        backingStore[process->getPID()] = std::make_pair(process->getCommandCounter(), process->getMemoryRequired());
        std::ofstream saveBackingStoreFile(".pagefile", std::ios::trunc);

        if (saveBackingStoreFile.is_open()) {
            for (const auto& entry : backingStore) {
                saveBackingStoreFile << entry.first << " " << entry.second.first << " " << entry.second.second << std::endl;
            }
        }
    }
}