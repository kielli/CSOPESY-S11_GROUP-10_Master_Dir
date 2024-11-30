#pragma once
#include <string>
#include <memory>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "../Process/Process.h"

class BackingStore {
private:
    std::string directory;

    // Ensures that the backing store directory exists
    void ensureDirectoryExists();

public:
    // Constructor
    BackingStore(const std::string& dir);

    // Save a process to the backing store
    void saveProcess(std::shared_ptr<Process> process);

    // Load a process from the backing store
    void loadProcess(std::shared_ptr<Process> process);

    // Remove a process file from the backing store
    void removeProcess(const std::shared_ptr<Process>& process);
};


