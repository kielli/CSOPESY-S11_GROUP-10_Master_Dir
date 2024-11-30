#pragma once
#include <conio.h>

class IKeyboardEvent
{
public:
	virtual void onKeyDown(char key) = 0;
	virtual void onKeyUp(char key) = 0;
};
