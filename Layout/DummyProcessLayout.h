#pragma once
#include <iostream>
#include <ctime>
#include <iomanip>
#include <Windows.h>
#include <memory>
#include <vector>

#include "../TypedefRepo.h"


// Define the GPUProcess structure to hold dummy data
struct GPUProcess {
    int gpu;
    String giid;
    String ciid;
    int pid;
    String type;
    String processName;
    String memoryUsage;
};

class DummyProcessLayout
{
public:
    void displayGPUSummaries() const;               // Displays the GPU summaries
    void displayProcessList(const std::vector<GPUProcess>& processes) const;  // Displays the process list
    void displayCurrentDateTimes() const;           // Displays the current date and time
};