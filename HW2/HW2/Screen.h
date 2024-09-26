#ifndef SCREEN_H
#define SCREEN_H

#include <string>
#include <vector>

class Screen {
protected:
    
    int currentLineNumber = 1;
    int totalLines = 1;

public:
    std::vector<std::string> contents;
    virtual void display() = 0;                                     // abstract method for displaying the screen
    virtual void handleCommand(const std::string& command) = 0;     // abstract method for handling commands
    virtual ~Screen() {}

    // stores and print a line 
    void printAndStore(const std::string& line);

    // store all content
    void StoreAll(const std::vector<std::string>& lines);

    void deleteContent(const std::vector<std::string>& lines);

    void displayHeader();

    //store a line
    void Store(const std::string& line);

    // redraws the screen by printing all stored contents
    void redrawScreen();

    // updates and returns the current line number
    int updateNumberOfLines() const;

    //returns the total number of lines
    int getTotalLines() const;

    //prints the processed command
    void print_process(const std::string& command);

    //prints error message
    void print_error(const std::string& command);
};

#endif

