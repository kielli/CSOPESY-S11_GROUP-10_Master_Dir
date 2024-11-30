#pragma once
#include <iostream>
#include <limits>
#include <memory>



#include "../TypeDefRepo.h"
#include "../Console/AConsole.h"
#include "../Process/Process.h"


class BaseScreen : public AConsole
{
public:
	BaseScreen(std::shared_ptr<Process> process, String processName);
	void onEnabled() override;
	void process() override;
	void display() override;

private:
	void printProcessInfo();
	std::shared_ptr<Process> attachedProcess;
	bool refreshed = false;
};

