#ifndef PROCESS_H
#define PROCESS_H

#include "Instruction.h"

#include <string>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

class Process {
	protected:
		string pName = "";
		int pId;
		int numInstructions = 0;
		int remainingI = 0;

		int arrivalT = 0;
		int burstT = 0;
		int waitingT = 0;
		int turnAroundT = 0;
	
	public:
		Process();
		virtual ~Process() {}

		Process(const string& pName, int processId, int numInstructions)
			: pName(pName), pId(processId), numInstructions(numInstructions), remainingI(numInstructions) {}

		string execute();
		string executeInstruction();
		string getPName();
		string getTimestamp() const;

		int getPID();
		int getTotalInstructions();
		int getArrivalT();
		int getWaitingT();
		int getTurnAroundT();
		int getRemainingI(); 

		void setNumInstructions(int num);
		void setArrivalT(int arrivalT);
		void setWaitingT(int waitingT);
		void setTurnAroundT(int turnAroundT);

		int hasFinished() const;
};

#endif