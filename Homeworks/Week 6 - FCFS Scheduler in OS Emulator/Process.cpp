#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

class Process{
private:
    string processName;
    int id;
    int totalInstructions;
    int remainingInstructions;

public:
    Process(const string& pName, int processId, int numInstructions)
        : processName(pName), id(processId), totalInstructions(numInstructions), remainingInstructions(numInstructions) {} 
    string executeInstruction(){
        string helloMessage = "Hello World from " + processName;
        string finishMessage = "Process " + to_string(id) + ": " + processName + " has finished";
        if(remainingInstructions > 0){
            remainingInstructions--;
            return helloMessage;
            
        } else {
            return finishMessage;
        }
    }

    int getRemainingInstructions(){
        return remainingInstructions;
    }

    int hasFinished() const {
        return remainingInstructions == 0;
    }

};
