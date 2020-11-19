#include "interface.h"
#include <conio.h>
#include <ctype.h>

string s = "";

int main() {
	Interface interfacer = Interface(140, 40, 60, 30);
	interfacer.set_titulo("Inventory");

	while (1) {
		bool updated = false;

		// Input
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

		// Design & Draw
		if (updated) {
			interfacer.interface_principal();
			interfacer.draw();
			updated = false;
		}
	}

	return 0;
}