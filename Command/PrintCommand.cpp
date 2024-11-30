#include "PrintCommand.h"

PrintCommand::PrintCommand(int pid, String& toPrint) : ICommand(pid, ICommand::PRINT)
{
	this->toPrint = toPrint;
}

void PrintCommand::execute()
{
	//ICommand::execute();

	std::cout << "PID " << this->pid << ": " << this->toPrint << std::endl;
	//std::stringstream msg; msg << String("PID ") << this->pid << ": " << this->toPrint << std::endl;

	//MessageBuffer::log(msg.str());
}

