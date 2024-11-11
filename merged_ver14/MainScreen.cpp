#include "MainScreen.h"

#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <atomic>
#include <random>
#include <cstdlib>   // for rand() and srand()
#include <ctime> 
#include <fstream>
#include <conio.h>

// // Main screen constructor
MainScreen::MainScreen(ScreenManager* manager) : screenManager(manager){}

// Displays the main screen header
void MainScreen::displayHeader() {
    cout << "  ____   ____    ____   _____   _____   ____ __    __\n";
    cout << " / ___] / ___]  / __ \\ |  __ \\ | ____] / ___]\\ \\  / /\n";
    cout << "| |    | (___  | /  \\ || |__) || |___ | (___  \\ \\/ /\n";
    cout << "| |     \\___ \\ | |  | || ____/ | ____] \\___ \\  \\  /\n";
    cout << "| \\___  ____) || \\__/ || |     | |___  ____) | |  |\n";
    cout << " \\____][_____/  \\____/ |_|     |_____][_____/  |__|\n\n";
    cout << "------------------------------------------------------------\n";
    cout << "\033[32m" << "Welcome to CSOPESY Emulator!\n\n"; // Green text

    cout << "\033[37m" << "Developers:\n";
    cout << "\t   Cipriaso, James Kielson\n";
    cout << "\t   Hallar, Francine Marie\n";
    cout << "\t   Hong, Letty\n";
    cout << "\t   Pe, Gyan Josh\n";

    cout << "\nLast updated: \n"; // TODO: include time stamp here
    cout << "\n------------------------------------------------------------\n";
    cout << "\033[33m" << "Type 'exit' to quit, 'clear' to clear the screen, 'initialize' to start program\n"; // Yellow text
    cout << "\033[37m" << "\n"; // Reset to white text
}

bool MainScreen::isExitRequested() const {
    return isExit;
}

void MainScreen::displayProcessSmi() {}