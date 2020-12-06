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
	int holding[100] = { 0 };
	bool updated = false;
	char typed;
	int const MAX_HOLD = 10;

	void setInput(int index, bool value);

	void specialHold(int inputt, int command);

public:
	enum inputCommands { begin, left, right, up, down, enter, space, backspace, deleter, tab, typing, end };
	bool getInput(int index);
	char getTyped();
	void input();
};