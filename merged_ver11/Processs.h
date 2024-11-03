
#ifndef PROCESSS_H
#define PROCESSS_H

#include "Instruction.h"
#include <string>
#include <vector>
#include <chrono>
#include <thread>


using namespace std;


class Processs {
protected:
	//chrono::time_point<std::chrono::system_clock> arrivalT;
	int arrivalT=0;
	int burstT=0;
	int waitingT=0;
	int turnAroundT=0;
	string pName="";
	int pId;
	int numInstructions = 0;
	int remainingI = 0;
	/*vector<Instruction> instructions;*/

public:
	Processs();
	virtual ~Processs() {}
	//Processs(const string& pName, int processId, int numInstructions) /*:numInstructions(numInstructions)*/ {};
	Processs(const string& pName, int processId, int numInstructions)
		: pName(pName), pId(processId), numInstructions(numInstructions), remainingI(numInstructions) {}
	/*void addInstruction(int numInstruction);*/
	string execute();
	string executeInstruction();
	string getPName();
	int getPID();
	int getTotalInstructions();
	void setNumInstructions(int num);
	void setArrivalT(int arrivalT);
	void setWaitingT(int waitingT);
	void setTurnAroundT(int turnAroundT);
	int getArrivalT();
	int getWaitingT();
	int getTurnAroundT();

	int hasFinished() const;

	int getRemainingI();

	/*double calculateBurstT(int CpuCycle, int batchprocessFreq);*/


};

#endif