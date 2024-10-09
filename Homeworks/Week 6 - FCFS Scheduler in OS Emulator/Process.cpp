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
    void executeInstruction(){
        if(remainingInstructions > 0){
            cout << "Hello World from " << id << ": " << processName << endl;
            remainingInstructions--;
            
        } else {
            cout << "Process " << id << ": " << processName << " has finished" << endl;
        }
    }

    int getRemainingInstructions(){
        return remainingInstructions;
    }

    int hasFinished() const {
        return remainingInstructions == 0;
    }

};

int main() {
    Process test("SampleProcess", 1, 10);

    while(!test.hasFinished()){
        test.executeInstruction();
    }

    cout << "Test Done";
}