#include "Screen.h"
#include <iostream>

void Screen::printAndStore(const std::string& line) {
    this->contents.push_back(line);
    std::cout << line << std::endl;
    this->totalLines++;
    this->currentLineNumber++;
}

void Screen::StoreAll(const std::vector<std::string>& lines) {
    for (const auto& line : lines) {
        this->contents.push_back(line);
        this->currentLineNumber++;
        this->totalLines++;
    }
}

void Screen::deleteContent(const std::vector<std::string>& lines) {
    this->contents.clear();
}

void Screen::displayHeader() {
    std::cout << "  ____   ____    ____   _____   _____   ____ __    __\n";
    std::cout << " / ___] / ___]  / __ \\ |  __ \\ | ____] / ___]\\ \\  / /\n";
    std::cout << "| |    | (___  | /  \\ || |__) || |___ | (___  \\ \\/ /\n";
    std::cout << "| |     \\___ \\ | |  | || ____/ | ____] \\___ \\  \\  /\n";
    std::cout << "| \\___  ____) || \\__/ || |     | |___  ____) | |  |\n";
    std::cout << " \\____][_____/  \\____/ |_|     |_____][_____/  |__|\n\n";
    std::cout << "Hello, Welcome to CSOPESY command line!\n";
    std::cout << "Type 'exit' to quit, 'clear' to clear the screen\n";
}

void Screen::Store(const std::string& line) {
    this->contents.push_back(line);
    this->totalLines++;
    this->currentLineNumber++;
}

void Screen::redrawScreen() {
    for (const auto& line : contents) {
        std::cout << line << std::endl;
    }
}

int Screen::updateNumberOfLines() const {
    return this->currentLineNumber;
}

int Screen::getTotalLines() const {
    return this->totalLines;
}

void Screen::print_process(const std::string& command) {
    std::cout << command << " command recognized. Doing something.\n";
}

void Screen::print_error(const std::string& command) {
    std::cout << "Unknown command: " << command << ".\n";
}

