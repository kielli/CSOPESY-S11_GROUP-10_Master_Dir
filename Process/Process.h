#pragma once
#include <memory>
#include <vector>
#include <chrono>
#include <mutex>

#include "../TypedefRepo.h"
#include "../Command/PrintCommand.h"

class Process
{
public:
    enum ProcessState {
        READY,
        RUNNING,
        WAITING,
        FINISHED
    };

    //changes: added reqMemory
    Process(int pid, String name, int totalLines, size_t reqMemory);

    void manualAddCommand(String command);

    void addCommand(ICommand::CommandType commandType);
    void executeCurrentCommand() const;
    void moveToNextLine();

    bool isFinished() const;
    int getRemainingTime() const;
    int getCommandCounter() const;
    size_t getLinesOfCode() const;
    int getPID() const;
    int getCPUCoreID() const;
    ProcessState getState() const;
    String getName() const;

    std::tm getArrivalTime() const;
    String getFormattedArrivalTime() const;

    void generateArrivalTime();

    void generateCommands();

    void setCPUCoreID(int cpuCoreID);

    //added
    size_t getRequiredMemory() const;
    void* getStoredAt() const;
    void setStoredAt(void* ptr);
    void setInBackingStore(bool state);
    bool isInBackingStore() const;
    void setMemorySize(size_t size);

private:
    int pid;
    String name;
    int totalLines;
    int commandCounter = 0;
    std::tm localArrivalTime; // Store the Arrival Time
    
    typedef std::vector<std::shared_ptr<ICommand>> CommandList;
    CommandList commandList;
    int cpuCoreID = -1;
    ProcessState currentState;

    // mutex for process
    std::mutex processMutex;

	friend class CPUCore;

    //added
    size_t requiredMemory;
    void* storedAt;
    bool inBackingstore;
};


