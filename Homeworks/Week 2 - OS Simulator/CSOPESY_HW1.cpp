//CSOPESY Homework #2
//S11 - Cipriaso, James Kielson S.

#include <iostream>
#include <algorithm>
#include <stdlib.h>
using namespace std;

void menu(int mode, string command){
    cout << " ██████╗███████╗ ██████╗ ██████╗ ███████╗███████╗██╗   ██╗\n";
    cout << "██╔════╝██╔════╝██╔═══██╗██╔══██╗██╔════╝██╔════╝╚██╗ ██╔╝\n";
    cout << "██║     ███████╗██║   ██║██████╔╝█████╗  ███████╗ ╚████╔╝ \n";
    cout << "██║     ╚════██║██║   ██║██╔═══╝ ██╔══╝  ╚════██║  ╚██╔╝  \n";
    cout << "╚██████╗███████║╚██████╔╝██║     ███████╗███████║   ██║   \n";
    cout << " ╚═════╝╚══════╝ ╚═════╝ ╚═╝     ╚══════╝╚══════╝   ╚═╝   \n" << endl;
    cout << "\033[32;1mWelcome to the CSOPESY Terminal!\033[0m" << endl;
    cout << "\033[33mType 'exit' to quit, Type 'clear to clear screen\033[0m" << endl;
    if(mode==1)
        cout << "'" << command << "'" << " is an invalid command!" << endl;
    else if(mode==0)
        cout << "'" << command << "'" << " command recognized. Doing something...." << endl;
}

int commands(string option){
    string commandArray[] = {"initialize", "screen", "scheduler-test", "scheduler-stop", "report-util"};
    string commandOutput = " command recognized. Doing something....";

    if(option=="clear")
        system("cls");
    else if(option=="exit"){
        cout << "Goodbye!";
        exit(0);
    }
    else if(find(commandArray, commandArray+5, option) != commandArray+5)
        return 0;
    else
        return 1;
    return -1;
}

int main(){
    int displayMode = 2;
    string displayCommand = " ";
    while(true){
        system("cls");
        menu(displayMode, displayCommand);
        string option;
        cout << "Enter command: ";
        cin >> option;
        cout << "\n";
        displayMode = commands(option);
        displayCommand = option;
        cout << "\n";
    }
}