#ifndef SCREEN_H
#define SCREEN_H

#include "Process.h"

#include <string>
#include <vector>

using namespace std;

// A screen is a process
class Screen : public Process {
    protected:
        int currentLineNumber = 1;
        int totalLines = 1;
        bool isRunning = true;
        bool isActive = false;

    public:
        virtual void display() = 0;                             // abstract method for displaying the screen
        virtual void handleCommand(const string& command) = 0; // abstract method for handling commands

        Screen() : Process(){}
        Screen(string pName, int pId, int numInstructions)
            : Process(pName, pId, numInstructions) {}

        vector<string> contents;

        void printAndStore(const string& line);
        void StoreAll(const vector<string>& lines);
        void deleteContent(const vector<string>& lines);

        void displayHeader();
        void Store(const string& line);
        void redrawScreen();

        int updateNumberOfLines() const;
        int getTotalLines() const;

        void print_process(const string& command);
        void print_error(const string& command);

        string getTimestamp() const;

        bool getRunningStatus();
        bool getActiveStatus();

        void setRunningStatusOff();
        void setActiveStatusOn();
        void setActiveStatusOff();
};

#endif