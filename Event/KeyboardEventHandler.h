#pragma once
#include <iostream>
#include <Windows.h>

#include "../TypeDefRepo.h"
#include "IKeyboardEvent.h"


class KeyboardEventHandler : public IKeyboardEvent
{
public:
	void onKeyDown(char key) override;

	void onKeyUp(char key) override;

	String getUserInput() const;

private:
	CharVector keyInputs;
	String userInput, storedInput;



	String convertCharVectorToString(CharVector& keyInputs);
};

