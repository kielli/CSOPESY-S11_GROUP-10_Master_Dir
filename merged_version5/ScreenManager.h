#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Screen.h"
#include "MainScreen.h"
#include "CreatedScreen.h"
#include "KeyboardEventHandler.h"

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

using namespace std;

class ScreenManager {
private:
    vector<unique_ptr<Screen>> screens;
    unordered_map<string, int> screenMap;
    int currentScreenIndex = 0;  // index of the current screen
    bool initialized = false;
    KeyboardEventHandler KEH;

    int numCPU = 0;
    string scheduler = "fcfs";
    int quantumCycles = 0;
    int batchProcessFreq = 0;
    int minInstructions = 0;
    int maxInstructions = 0;
    int delayPerExec = 0;
    int cpuCycleCounter = 0;


public:
    ScreenManager();
    void initialize(int numCPU, string scheduler, int quantumCycles, int batchProcessFreq, int minInstructions, int maxInstructions, int delayPerExec);
    bool getInitializeState();
    void createScreen(const string& screenName);
    void createDummyScreen(const std::string& screenName);
    void resumeScreen(const string& screenName);
    void switchToMainScreen();    // switch back to the main screen
    void displayCurrentScreen();
    bool isMainScreenExitRequested();
    void addContent(const string& content);
    void handleCurrentCommand(const string& command);
    int getTotalProcess();
    int getCpuCounterCycle();
    void setCpuCounterCycle(int cpuCycle);
    int getbatchProcessFreq();
    KeyboardEventHandler getKEH();
    void PollKeyboard(KeyboardEventHandler KEH);
};

#endif