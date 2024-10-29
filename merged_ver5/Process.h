
#ifndef PROCESS_H
#define PROCESS_H

#include "Instruction.h"

#include <string>
#include <vector>

using namespace std;

class Process {
protected:
	int arrivalT;
	int burst;
	int waitingT;
	int turnAroundT;
	string pName;
	int pId;

	vector<Instruction> instructions;

public:
	virtual ~Process() {}
	
	void addInstruction(int numInstruction);
	void execute();
	string getPName();
	int getPID();
};

#endif