#include "interface.h"
#include "input.h"
#include "IObserver.h"
#include <conio.h>
#include <ctype.h>

Input input = Input();
Interface interfacer = Interface(140, 40, 60, 30);

class Main : public IObserver {
public:
	void update() override {
		// Input
		if (input.getInput(input.left))
			interfacer.goLeft();

		if (input.getInput(input.right))
			interfacer.goRight();

		if (input.getInput(input.up))
			interfacer.goUp();

		if (input.getInput(input.down))
			interfacer.goDown();

		if (input.getInput(input.space))
			interfacer.space();

		if (input.getInput(input.backspace))
			interfacer.backspace();

		if (input.getInput(input.enter))
			interfacer.subirLinha();

		if (input.getInput(input.typing))
			interfacer.addLetra(input.getTyped());


		// Design & Draw
		interfacer.interfacePrincipal();
		interfacer.draw();
	}


	int main() {
		interfacer.setTitulo("Inventory");

		while (1) {
			input.input();
			interfacer.clocking();
		}

		return 0;
	}
};

int main() {
	Main m = Main();
	input.add(&m);
	m.main();
};