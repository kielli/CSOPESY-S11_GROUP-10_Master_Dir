#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include <vector>

using namespace std;

class Instruction {

protected:
	int Iid;
	string helloMessage = "Hello World from ";

public:
	~Instruction() {};
	Instruction(int id): Iid(id) {};
};

#endif