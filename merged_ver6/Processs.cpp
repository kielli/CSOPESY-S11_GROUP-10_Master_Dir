#include "Processs.h"



//Processs::Processs(const string& pName, int processId, int numInstructions) : pName(pName),
//pId(processId), numInstructions(numInstructions), remainingI(numInstructions) {}

//void Processs::addInstruction(int numInstruction)
//{
//	for (int i = 0; i < numInstruction; i++) {
//		instructions.push_back(this->instructions.size());
//	}
//}

Processs::Processs()
{
}

string Processs::execute()
{
	string helloMessage = "Hello World from " /*+ processName*/;
	string finishMessage = "Process "/* + to_string(id) + ": " + processName*/ /*+ " has finished"*/;

	if (remainingI > 0) {
		remainingI--;
		return helloMessage;
	}
	else {
		return finishMessage;
	}
}

string Processs::executeInstruction() {
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	string helloMessage = "Hello World from " + pName;
	string finishMessage = "Process " + to_string(pId) + ": " + pName + " has finished";

	if (remainingI > 0) {
		remainingI--;
		return helloMessage;
	}
	else {
		return finishMessage;
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

int Processs::getTotalInstructions() {
	return this->numInstructions;
}

void Processs::setNumInstructions(int num)
{
	this->numInstructions = num;
}

void Processs::setArrivalT(int arrivalT)
{
	this->arrivalT = arrivalT;
}

void Processs::setWaitingT(int waitingT)
{
	this->waitingT = waitingT;
}

void Processs::setTurnAroundT(int turnAroundT)
{
	this->turnAroundT = turnAroundT;
}

int Processs::getArrivalT()
{
	return this->arrivalT;
}

int Processs::getWaitingT()
{
	return this->waitingT;
}

int Processs::getTurnAroundT()
{
	return this->turnAroundT;
}

int Processs::hasFinished() const {
	return remainingI == 0;
}

//double Processs::calculateBurstT(int CpuCycle, int batchprocessFreq)
//{
//	return (this->numInstructions * CpuCycle) / batchprocessFreq;
//}
