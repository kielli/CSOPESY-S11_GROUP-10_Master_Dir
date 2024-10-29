
#ifndef PROCESSS_H
#define PROCESSS_H

#include "Instruction.h"
#include <string>
#include <vector>

using namespace std;


class Processs {
protected:
	int arrivalT;
	int burst;
	int waitingT;
	int turnAroundT;
	string pName;
	int pId;
	vector<Instruction> instructions;

public:
	virtual ~Processs() {}
	void addInstruction(int numInstruction);
	void execute();
	string getPName();
	int getPID();
};

#endif