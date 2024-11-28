# CSOPESY-Process-Scheduler

### To run the program (w/out Memory Allocator implementation)
g++ -std=c++20 Command/PrintCommand.cpp Config/GlobalConfig.cpp Console/AConsole.cpp Console/ConsoleManager.cpp Console/MainConsole.cpp Console/MarqueeConsole.cpp Event/KeyboardEventHandler.cpp Layout/DummyProcessLayout.cpp Log/MessageBuffer.cpp Process/Process.cpp Scheduler/CPUCore.cpp Scheduler/Scheduler.cpp Screen/BaseScreen.cpp main.cpp -o main
