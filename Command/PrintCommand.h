#pragma once
#include "ICommand.h"
#include "../TypeDefRepo.h"
#include "../Log/MessageBuffer.h"


class PrintCommand : public ICommand
{
public:
	PrintCommand(int pid, String& toPrint);
	void execute() override;

private:
	String toPrint;
};

