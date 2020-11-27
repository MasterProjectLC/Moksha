#include "input.h"

// Input
bool Input::getInput(int index) {
	return inputs[index];
};

void Input::setInput(int index, bool value) {
	inputs[index] = value;
	updated = value;
}

bool is_key_pressed(int key) {
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


		if (is_key_pressed(VK_RETURN))
			pressed[enter] = true;
		
		// Typing
		if (!updated) {
			int c = _getch();
			if (c == 0xe0)
				_getch();
			
			else if (isalnum(c)) {
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

	// Handle not pressing
	if (pressed[enter] == true && !is_key_pressed(VK_RETURN)) {
		pressed[enter] = false;
		setInput(enter, true);
	}

	if (!updated)
		return;

	notify();
	for (int i = begin+1; i != end; i++) {
		setInput(i, false);
	}
};

void Input::specialHold(int inputt, int command) {
	if (is_key_pressed(inputt)) {
		if (holding[command] == 0 || holding[command] > MAX_HOLD)
			setInput(command, true);
		holding[command]++;
	}
	else
		holding[command] = 0;
}