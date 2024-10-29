#include "Process.h"

void Process::addInstruction(int numInstruction) {
	for (int i = 0; i < numInstruction; i++) {
		instructions.push_back(this->instructions.size());
	}
}

string Process::getPName() {
	return this->pName;
}

int Process::getPID() {
	return this->pId;
}