#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Screen.h"
#include "MainScreen.h"
#include "CreatedScreen.h"
#include "FCFS_Scheduler.h"
#include "RR_Scheduler.h"
#include "CPU_Core.h"

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <fstream>
using namespace std;

class ScreenManager {
  private:
    vector<unique_ptr<Screen>> screens;
    unordered_map<string, int> screenMap;
    int currentScreenIndex = 0;  // index of the current screen
    bool initialized = false;
    int cpunum, quantum=0, batchfreq, minIn, maxIn, delay=0;
    string schedType;
    vector<CPU_Core> cpuList;
    vector<Process> processList;
    thread schedulerThreadR;
    RR_Scheduler rscheduler;
    /*RR_Scheduler rscheduler = RR_Scheduler(this->quantum, this->delay);
    FCFS_Scheduler fScheduler = FCFS_Scheduler(this->delay);*/
  public:
      ScreenManager();

      void createScreen(const string& screenName);
      void resumeScreen(const string& screenName);
      
      void switchToMainScreen();    // switch back to the main screen
      void displayCurrentScreen();  // display the current screen

      bool isMainScreenExitRequested();
      void addContent(const string& content);
      void handleCurrentCommand(const string& command);
      void createCores();
      void createProcess();
      void runScheduler();
      void PollKeyboard(KeyboardEventHandler KEH);
};

#endif
