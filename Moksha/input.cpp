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
		if (is_key_pressed(VK_LEFT))
			setInput(left, true);

		if (is_key_pressed(VK_RIGHT))
			setInput(right, true);

		if (is_key_pressed(VK_UP))
			setInput(up, true);

		if (is_key_pressed(VK_DOWN))
			setInput(down, true);

		if (is_key_pressed(VK_SPACE)) {
			if (holding_space == 0 || holding_space > MAX_HOLD)
				setInput(space, true);
			holding_space++;
		} else
			holding_space = 0;

		if (is_key_pressed(VK_BACK)) {
			if (holding_back == 0 || holding_back > MAX_HOLD)
				setInput(backspace, true);
			holding_back++;
		} else
			holding_back = 0;

		if (is_key_pressed(VK_DOWN))
			setInput(down, true);

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