#include "BackingStore.h"

// Constructor initializes the backing store directory
BackingStore::BackingStore(const std::string& dir) : directory(dir) {
    ensureDirectoryExists();
}

// Ensures the directory for the backing store exists
void BackingStore::ensureDirectoryExists() {
    std::filesystem::path fullDirectoryPath = std::filesystem::path(std::filesystem::current_path()) / directory;

    if (!std::filesystem::exists(fullDirectoryPath)) {
        if (std::filesystem::create_directories(fullDirectoryPath)) {
            std::cout << "Directory created: " << fullDirectoryPath << "\n";
        }
        else {
           /* std::cerr << "Failed to create directory: " << fullDirectoryPath << "\n";*/
        }
    }
    else {
        /*std::cout << "Directory already exists: " << fullDirectoryPath << "\n";*/
    }

    directory = fullDirectoryPath.string();
}

// Save process to a file in the backing store
void BackingStore::saveProcess(std::shared_ptr<Process> process) {
    std::string filePath = directory + "/" + process->getName() + ".txt";
    std::ofstream outFile(filePath, std::ios::binary);

    if (outFile.is_open()) {
        // Serialize process data
        size_t memorySize = process->getRequiredMemory();
        outFile.write(reinterpret_cast<const char*>(&memorySize), sizeof(memorySize));
        // Add more attributes to serialize as needed
        outFile.close();

        process->setInBackingStore(true);
        /*std::cout << "Process " << process->getName() << " saved to backing store.\n";*/
    }
    else {
        /*std::cerr << "Failed to save process " << process->getName() << " to backing store.\n";*/
    }
}

// Load process from the backing store
void BackingStore::loadProcess(std::shared_ptr<Process> process) {
    std::string filePath = directory + "/" + process->getName() + ".txt";
    std::ifstream inFile(filePath, std::ios::binary);

    if (inFile.is_open()) {
        // Deserialize process data
        size_t memorySize;
        inFile.read(reinterpret_cast<char*>(&memorySize), sizeof(memorySize));
        process->setMemorySize(memorySize);
        // Restore more attributes as needed
        inFile.close();

        process->setInBackingStore(false);
       /* std::cout << "Process " << process->getName() << " loaded from backing store.\n";*/
    }
    else {
       /* std::cerr << "Failed to load process " << process->getName() << " from backing store.\n";*/
    }
}

// Remove process file from the backing store
void BackingStore::removeProcess(const std::shared_ptr<Process>& process) {
    std::string filePath = directory + "/" + process->getName() + ".txt";
    if (std::filesystem::exists(filePath)) {
        std::filesystem::remove(filePath);
       /* std::cout << "Process " << process->getName() << " removed from backing store.\n";*/
    }
}



