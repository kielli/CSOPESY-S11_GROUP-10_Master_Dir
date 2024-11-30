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

        Process(int pid, String name, int totalLines, size_t memoryRequired);

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
        size_t getMemoryRequired() const;
        char* getMemBaseAddress() const;

        void generateArrivalTime();

        void generateCommands();

        void setCPUCoreID(int cpuCoreID);
        void setMemBaseAddress(char* memBaseAddress);


    private:
        int pid;
        String name;
        int totalLines;
        size_t memoryRequired;
        char* memBaseAddress;
        
        int commandCounter = 0;
        std::tm localArrivalTime; // Store the Arrival Time

        typedef std::vector<std::shared_ptr<ICommand>> CommandList;
        CommandList commandList;
        int cpuCoreID = -1;
        ProcessState currentState;

        // mutex for process
        std::mutex processMutex;

        friend class CPUCore;
};