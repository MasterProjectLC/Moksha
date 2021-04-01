#pragma once
#include <windows.h>
#include <winuser.h>
#include <conio.h>
#include <ctype.h>
#include "IObservable.h"

using namespace std;

class Input : public IObservable {
private:
	int const MAX_HOLD = 15;
	bool inputs[100];
	bool pressed[100];
	int holding[100];
	bool updated;
	char typed;

	void setInput(int index, bool value);

	void specialHold(int inputt, int command);
	void specialHold(int inputt, int command, boolean once);

public:
	Input();

	enum inputCommands { begin, left, right, up, down, enter, space, backspace, deleter, tab, typing, escape, end };
	bool getInput(int index);
	char getTyped();
	void input();
};