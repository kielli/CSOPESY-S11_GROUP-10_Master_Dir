#include "Processs.h"

void Processs::addInstruction(int numInstruction)
{
	for (int i = 0; i < numInstruction; i++) {
		instructions.push_back(this->instructions.size());
	}
}

string Processs::getPName()
{
	return this->pName;
}

int Processs::getPID()
{
	return this->pId;
}