#include "KeyboardEventHandler.h"

void KeyboardEventHandler::onKeyDown(char key)
{
	if (GetAsyncKeyState(VK_BACK) & 0x8000 && !keyInputs.empty())
	{
		keyInputs.pop_back();
	}
	else if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		userInput = storedInput;
		keyInputs.clear();
		storedInput.clear();
	}
	else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		keyInputs.push_back(' ');
	}
	else if (GetAsyncKeyState(VK_SHIFT) & 0x8000 || GetAsyncKeyState(VK_CAPITAL) & 0x8000) { //Manages Shift button event
		keyInputs.push_back(key);
	}
	else
	{
		keyInputs.push_back(key);
	}

	storedInput = convertCharVectorToString(keyInputs);
}

void KeyboardEventHandler::onKeyUp(char key)
{
	keyInputs.push_back(key);
	storedInput = convertCharVectorToString(keyInputs);
}

String KeyboardEventHandler::getUserInput() const
{
	return this->userInput;
}

String KeyboardEventHandler::convertCharVectorToString(CharVector& keyInputs)
{
	return String(keyInputs.begin(), keyInputs.end());
}


