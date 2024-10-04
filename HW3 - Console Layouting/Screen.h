#ifndef SCREEN_H
#define SCREEN_H

#include <string>
#include <vector>

using namespace std;

class Screen {
    protected:    
        int currentLineNumber = 1;
        int totalLines = 1;

    public:
        vector<string> contents;

        virtual void display() = 0;                             // abstract method for displaying the screen
        virtual void handleCommand(const string& command) = 0; // abstract method for handling commands
        virtual ~Screen() {}

        // Stores and print a line 
        void printAndStore(const string& line);

        // Stores all content
        void StoreAll(const vector<string>& lines);

        // Deletes content
        void deleteContent(const vector<string>& lines);

        void displayHeader();

        //Stores a line
        void Store(const string& line);

        // Redraws the screen by printing all stored contents
        void redrawScreen();

        // Updates and returns the current line number
        int updateNumberOfLines() const;

        // Returns the total number of lines
        int getTotalLines() const;

        // Prints the processed command
        void print_process(const string& command);

        // Prints error message
        void print_error(const string& command);
};

#endif