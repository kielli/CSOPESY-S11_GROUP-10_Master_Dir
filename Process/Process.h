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

        Process(int pid, String name, int totalLines);

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
};