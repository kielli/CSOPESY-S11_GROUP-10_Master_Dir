#include "MainScreen.h"
#include "ScreenManager.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

// Main screen constructor
MainScreen::MainScreen(ScreenManager* manager) : screenManager(manager) {
    commands["screen"] = [this](const string& args) {
        stringstream iss(args);
        string option, screenName;

        iss >> option >> screenName;

        if (option == "-s" && !screenName.empty()) {
            screenManager->createScreen(screenName);
        }
        else if (option == "-r" && !screenName.empty()) {
            screenManager->resumeScreen(screenName);
        }
        else {
            this->printAndStore("Invalid command format. Use: screen -s screenname or screen -r screenname\n");
        }
        };

    commands["initialize"] = [this](const string& args) {
        this->printAndStore("initialize command recognized. Doing something.\n");
        };

    commands["scheduler-test"] = [this](const string& args) {
        this->printAndStore("scheduler-test command recognized. Doing something.\n");
        };

    commands["scheduler-stop"] = [this](const string& args) {
        this->printAndStore("scheduler-stop command recognized. Doing something.\n");
        };

    commands["report-util"] = [this](const string& args) {
        this->printAndStore("report-util command recognized. Doing something.\n");
        };

    commands["clear"] = [this](const string& args) {
        this->deleteContent(this->contents);
        this->display();
        };

    commands["exit"] = [this](const string&) {
        cout << "Exiting program...\n";
        isExit = true;
        exit(0);
        };

    commands["nvidia-smi"] = [this](const string&) {
        this->Print_nvidia_smi_Header();
        };
}

// Displays the main screen header
void MainScreen::displayHeader() {
    cout << "  ____   ____    ____   _____   _____   ____ __    __\n";
    cout << " / ___] / ___]  / __ \\ |  __ \\ | ____] / ___]\\ \\  / /\n";
    cout << "| |    | (___  | /  \\ || |__) || |___ | (___  \\ \\/ /\n";
    cout << "| |     \\___ \\ | |  | || ____/ | ____] \\___ \\  \\  /\n";
    cout << "| \\___  ____) || \\__/ || |     | |___  ____) | |  |\n";
    cout << " \\____][_____/  \\____/ |_|     |_____][_____/  |__|\n\n";
    cout << "\033[32m" << "Hello, welcome to CSOPESY commandline!\n"; // Green text
    cout << "\033[33m" << "Type 'exit' to quit, 'clear' to clear the screen\n"; // Yellow text
    cout << "\033[37m" << "\n"; // Reset to white text
}


// Displays the header and the redrawn screen
void MainScreen::display() {
    system("cls");
    this->displayHeader();
    this->redrawScreen();
}

string truncateText(const string& text, size_t maxLength) {
    if (text.length() > maxLength) {
        return text.substr(0, maxLength - 3) + "...";  // Truncate and add "..."
    }
    return text;
}

void MainScreen::Print_nvidia_smi_Header() {
    this->deleteContent(this->contents);

    vector<string> nvidiaSmiOutput = {
       this->getTimestamp(),
        "+------------------------------------------------------------------------------+",
        "| NVIDIA-SMI 460.32.03    Driver Version: 460.32.03    CUDA Version: 11.2      |",
        "|-------------------------------+----------------------+-----------------------|",
        "| GPU  Name        Persistence-M | Bus-Id        Disp.A | Volatile Uncorr. ECC |",
        "| Fan  Temp  Perf  Pwr:Usage/Cap |         Memory-Usage | GPU-Util  Compute M. |",
        "|                                |                      |               MIG M. |",
        "|================================+======================+======================|",
        "|   0  Tesla T4            Off   | 00000000:00:1E.0 Off |                    0 |",
        "| N/A   52C    P8    20W /  70W  |    150MiB / 15109MiB |      0%      Default |",
        "|                                |                      |                  N/A |",
        "+-------------------------------+----------------------+----------------------+",
        "",
        "+------------------------------------------------------------------------------+",
        "| Processes:                                                  GPU Memory       |",
        "|  GPU       PID   Type   Process name                        Usage            |",
        "|==============================================================================|"
    };

    // List of dummy processes with truncation
    vector<tuple<int, int, string, int>> processes = {
        {0, 1234, "dummy_process_very_long_name_1", 200},
        {0, 5678, "dummy_process_2", 150},
        {0, 9012, "dummy_process_name_3_with_long_name", 100},
        {0, 3456, "dummy_process_4", 250},
        {0, 7890, "dummy_process_5", 300}
    };

    // Add process info to the output vector
    for (const auto& process : processes) {
        int gpu = get<0>(process);
        int pid = get<1>(process);
        string processName = truncateText(get<2>(process), 30);  // Truncate long names to 30 chars
        int memoryUsage = get<3>(process);

        // Format the process info as a string and store it
        stringstream ss;
        ss << "| " << setw(4) << gpu
            << "     " << setw(6) << pid
            << "     C   "  
            << left << setw(30) << processName  // 
            << right << setw(9) << memoryUsage << "MiB           |";
        nvidiaSmiOutput.push_back(ss.str());
    }

    nvidiaSmiOutput.push_back("+------------------------------------------------------------------------------+");

    // Store all the formatted output in the screen's content
    this->StoreAll(nvidiaSmiOutput);

    // Redraw the screen with the new content
    this->redrawScreen();
}

void MainScreen::handleCommand(const string& command) {
    istringstream iss(command);
    string cmd, args;

    iss >> cmd;
    getline(iss, args);

    if (commands.find(cmd) != commands.end()) {
        commands[cmd](args);  // pass the rest of the input as arguments
    }
    else {
        this->printAndStore("Unknown command: " + command);
    }
}

bool MainScreen::isExitRequested() const {
    return isExit;
}

int MainScreen::countScreens() const {
    return screenCount;
}

string MainScreen::getTimestamp() const {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm localTime; //tm structure to hold the local time

    // use localtime_s for thread safety
    localtime_s(&localTime, &currentTime);

    stringstream ss;
    ss << put_time(&localTime, "%a %b %d %H:%M:%S %Y");
    return ss.str();
}
