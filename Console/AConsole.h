#pragma once
#include "../TypeDefRepo.h"

class AConsole
{
public:
	AConsole(String name);
	~AConsole() = default;

	String getName() const;
	virtual void onEnabled() = 0;			// Calls when the screen shows up the first time
	virtual void display() = 0;				// Calls the screen per frame
	virtual void process() = 0;				// Certain process that the screen needs to do

	String name;
	friend class ConsoleManager;
};