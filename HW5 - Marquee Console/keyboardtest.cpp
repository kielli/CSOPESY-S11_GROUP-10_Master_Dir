#include <iostream>
#include <conio.h>
#include <Windows.h>
//Keyboard event interface
using namespace std;
class IKeyboardEvent {
public:
	virtual void OnKeyDown(char key) = 0;
	virtual void OnKeyUp(char key) = 0;
};
//Keyboard event implementation
class KeyboardEventHandler : public IKeyboardEvent {
public:
	void OnKeyDown(char key) override {
		std::cout<<"Key Down: " << key << std::endl;
	}
	void OnKeyUp(char key) override {
		std::cout<<"Key Up: " << key << std::endl;
	}
};
//Keyboard Polling
void PollKeyboard(IKeyboardEvent& keyboardEvent){
	while (true) {
		if(_kbhit()){
			char key = _getch();
			if(GetAsyncKeyState(key) & 0x8000){
				keyboardEvent.OnKeyDown(key);
			} else{
				keyboardEvent.OnKeyUp(key);
			}
		}
		//Other programs here marquee probably
	}
}

/*void keyboardInterruptHandler(){
	if(_kbhit()){
		char key = _getch();
		std::cout << "Key pressed: " << key << std::endl;
	}
}*/
int main(){
	KeyboardEventHandler keyboardHandler;
	
	PollKeyboard(keyboardHandler);
	return 0;
	/*while(true){
		Sleep(100);
		keyboardInterruptHandler();
	}*/
	return 0;
}