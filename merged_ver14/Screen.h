#ifndef SCREEN_H
#define SCREEN_H

#include "Process.h"

#include <string>
#include <vector>

using namespace std;

class ScreenManager;
class Process;

// A screen is a process
class Screen : public Process {
    protected:
        int currentLineNumber = 1;
        int totalLines = 1;

        bool isActive = false;
        bool isRunning = true;
        bool hasFinished = false;

        ScreenManager* screenManager;
        // vector<string> contents;

    public:
        Screen() : Process() {}

        Screen(string pName, ScreenManager* manager, int pId, int numInstructions, size_t requiredMemory)
            : Process(pName, pId, numInstructions, requiredMemory) {}

        virtual void display();                            // abstract method for displaying the screen
        virtual void displayHeader() = 0;
        virtual void redrawScreen();
        virtual void displayProcessSmi() = 0;

        vector<string> contents;

        void printAndStore(const string& line);
        void deleteContent();
        void Store(const string& line);

        int updateNumberOfLines() const;
        int getTotalLines() const;

        void print_process(const string& command);
        void print_error(const string& command);

        bool getActiveStatus();

        void setRunningFalse();
        void setActiveStatusOn();
        void setActiveStatusOff();

        bool getFinishState();
};

#endif