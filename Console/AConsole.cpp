#include "AConsole.h"

AConsole::AConsole(String name)
{
	this->name = name;
}

String AConsole::getName() const
{
	return this->name;
}
