#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Screen.h"
#include "MainScreen.h"
#include "CreatedScreen.h"

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

  public:
      ScreenManager();
      void createScreen(const string& screenName);
      void resumeScreen(const string& screenName);
      void switchToMainScreen();    // switch back to the main screen
      void displayCurrentScreen();  // display the current screen
      bool isMainScreenExitRequested();
      void addContent(const string& content);
      void handleCurrentCommand(const string& command);
      void CreateMarqueeConsole();
};

#endif
