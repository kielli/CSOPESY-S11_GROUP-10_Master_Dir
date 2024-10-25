#ifndef SCREEN_H
#define SCREEN_H

#include <string>
#include <vector>
#include <thread>
 
using namespace std;

class Screen {
    protected:
        int currentLineNumber = 1;
        int totalLines = 1;

    public:
        virtual ~Screen() {}
        virtual void displayScreen() = 0; // abstract method for displaying the screen
        virtual void handleCommand(const string& command) = 0; // abstract method for handling commands

        vector<string> contents;

        void addLine(const string& line);
        void printAndStore(const string& line);
        void Store(const string& line);

        void deleteContent(const vector<string>& lines);
        int updateNumberOfLines() const;
        int getTotalLines() const;

        void print_process(const string& command);
        void print_error(const string& command);

        void redrawScreen();
        string getTimestamp() const;
};

#endif