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
	int holding_space = 0;
	int holding_back = 0;
	int const MAX_HOLD = 10;

	void setInput(int index, bool value);

public:
	enum inputCommands { begin, left, right, up, down, enter, space, backspace, typing, end };
	bool getInput(int index);
	char getTyped();
	void input();
};