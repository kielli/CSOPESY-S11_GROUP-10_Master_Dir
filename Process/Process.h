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
	struct RequirementFlags {
		bool requireFiles;
		int numFiles;
		bool requireMemory;
		int memoryRequired;
	};


    enum ProcessState {
        READY,
        RUNNING,
        WAITING,
        FINISHED
    };

    Process(int pid, String name, int totalLines, RequirementFlags flags);

    void manualAddCommand(String command);

    void addCommand(ICommand::CommandType commandType);
    void executeCurrentCommand() const;
    void moveToNextLine();

    bool isFinished() const;
    int getRemainingTime() const;
    int getCommandCounter() const;
    int getLinesOfCode() const;
    int getPID() const;
    int getCPUCoreID() const;
    ProcessState getState() const;
    String getName() const;

    std::tm getArrivalTime() const;
    String getFormattedArrivalTime() const;

    void generateArrivalTime();

    void generateCommands();

    void setCPUCoreID(int cpuCoreID);

    void setBaseAddress(char* baseAddress);
    char* getBaseAddress() const;

    size_t getMemorySize() const;

    size_t getNumberOfPages() const;


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
    RequirementFlags requirementFlags;
    char* baseAddress;

    // mutex for process
    std::mutex processMutex;

	friend class CPUCore; 
};


