#pragma once
#include <windows.h>
#include <winuser.h>
#include <conio.h>
#include <ctype.h>
#include "IObservable.h"

using namespace std;

class Input : public IObservable {
private:
	bool inputs[100] = {false};
	bool pressed[100] = {false};
	bool updated = false;
	char typed;

	void setInput(int index, bool value);

public:
	enum inputCommands { left, right, up, down, enter, typing };
	bool getInput(int index);
	char getTyped();
	void input();
};