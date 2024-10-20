
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>
//#include <WinUser.h>

#include "Screen.h"

void Screen::printAndStore(const string& line) {
    this->contents.push_back(line);
    cout << line << std::endl;
    this->totalLines++;
    this->currentLineNumber++;
}

void Screen::StoreAll(const vector<string>& lines) {
    for (const auto& line : lines) {
        this->contents.push_back(line);
        this->currentLineNumber++;
        this->totalLines++;
    }
}

void Screen::deleteContent(const vector<string>& lines) {
    this->contents.clear();
}

void Screen::Store(const string& line) {
    this->contents.push_back(line);
    this->totalLines++;
    this->currentLineNumber++;
}

void Screen::redrawScreen() {
    for (const auto& line : contents) {
        cout << line << endl;
    }
}

int Screen::updateNumberOfLines() const {
    return this->currentLineNumber;
}

int Screen::getTotalLines() const {
    return this->totalLines;
}

void Screen::print_process(const string& command) {
    this->printAndStore(command + "command recognized. Doing something.");
}

void Screen::print_error(const string& command) {
    this->printAndStore("Unknown command: " + command);
}

void Screen::createProcessesAndCores() {
    this->processList.clear();
    this->cpuList.clear();

    // Create processes and CPU Cores
    for (int i = 0; i < 10; ++i) {
        processList.push_back(Process("process_" + std::to_string(i), i, 100));
        cout << "Debugging Created: process_" << i << endl; // Debugging line
    }

    for (int i = 0; i < 4; ++i) {
        cpuList.push_back(CPU_Core(i));
        cout << "Debugging Core: " << i << endl; // Debugging line
    }
}

void Screen::startSchedulerFCFS(FCFS_Scheduler& scheduler, vector<Process>& processes, vector<CPU_Core>& cores) {
    schedulerThread = thread ([&scheduler, &processes, &cores]() {
        scheduler.runScheduler(processes, cores);
    });
}

void Screen::stopSchedulerFCFS() {
    if (schedulerThread.joinable()) {
        schedulerThread.join();
    }
}