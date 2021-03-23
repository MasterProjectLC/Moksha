#include "input.h"

// Input
Input::Input() {
	inputs[100] = { false };
	pressed[100] = { false };
	holding[100] = { 0 };
	updated = false;
}

bool Input::getInput(int index) {
	return inputs[index];
};

void Input::setInput(int index, bool value) {
	inputs[index] = value;
	updated = value;
}

bool isKeyPressed(int key) {
	if (GetAsyncKeyState(key) & 0x8000)
		return true;
	return false;
}

char Input::getTyped() {
	return typed;
}

void Input::input() {
	updated = false;
	
	// Handle pressing
	if (_kbhit()) {
		// Single input
		specialHold(VK_LEFT, left);
		specialHold(VK_RIGHT, right);
		specialHold(VK_UP, up);
		specialHold(VK_DOWN, down);
		specialHold(VK_SPACE, space);
		specialHold(VK_BACK, backspace);
		specialHold(VK_DELETE, deleter);
		specialHold(VK_TAB, tab, true);
		specialHold(VK_ESCAPE, escape, true);
		specialHold(VK_RETURN, enter, true);
		
		// Typing
		if (!updated) {
			int c = _getch();
			if (c == 0xe0)
				_getch();
			
			else if (isalnum(c) || c == '\'') {
				typed = c;
				setInput(typing, true);
			}
		}

	}
	else {
		for (int i = begin + 1; i != end; i++) {
			holding[i] = 0;
		}
	}

	if (!updated)
		return;

	notify();
	for (int i = begin+1; i != end; i++) {
		setInput(i, false);
	}
};

void Input::specialHold(int inputt, int command) {
	specialHold(inputt, command, false);
}

void Input::specialHold(int inputt, int command, boolean once) {
	if (isKeyPressed(inputt)) {
		if (holding[command] == 0 || (!once && holding[command] > MAX_HOLD))
			setInput(command, true);
		holding[command]++;
	}
	else
		holding[command] = 0;
}