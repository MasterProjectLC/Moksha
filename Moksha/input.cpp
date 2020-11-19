#include "input.h"

// Input
template<typename Function>
bool Input::input(Function left, Function right, Function up, Function down) {
	bool updated = false;

	if (!_kbhit())
		continue;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		interfacer.go_left();
		updated = true;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		interfacer.go_right();
		updated = true;
	}

	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		interfacer.go_up();
		updated = true;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		interfacer.go_down();
		updated = true;
	}

	if (updated == false) {
		int c = _getch();

		if (c == 0xe0) {
			_getch();
			continue;
		}

		if (isalnum(c))
			s += c;
		if (c == 'c') {
			interfacer.add_linha(s);
			updated = true;
			s = "";
		}
	}

	return updated;
}