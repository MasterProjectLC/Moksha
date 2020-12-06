#include "interface.h"
#include "input.h"
#include "jogo.h"
//#include "fileManager.h"
#include "IObserver.h"
#include <conio.h>
#include <ctype.h>

Input input = Input();
Interface interfacer = Interface(140, 40, 60, 30);
//FileManager fileManager = FileManager();
Jogo jogo = Jogo();

class Overseer : public IObserver {
public:
	void update(int id) override {
		// Update
		switch (id) {
		case 0:
			inputUpdate();
			break;

		case 1:
			jogoUpdate();
		}

		// Design & Draw
		interfacer.interfacePrincipal();
		interfacer.draw();

	}

	void inputUpdate() {
		if (input.getInput(input.left))
			interfacer.pointerLeft();

		if (input.getInput(input.right))
			interfacer.pointerRight();

		if (input.getInput(input.up))
			interfacer.pointerUp();

		if (input.getInput(input.down))
			interfacer.pointerDown();

		if (input.getInput(input.space))
			interfacer.space();

		if (input.getInput(input.backspace))
			interfacer.removeLetra(true);

		if (input.getInput(input.deleter))
			interfacer.removeLetra(false);

		if (input.getInput(input.enter)) {
			jogo.receberArgs(interfacer.subirLinha());
		}

		if (input.getInput(input.tab))
			interfacer.setTab(!interfacer.getTab());

		if (input.getInput(input.typing))
			interfacer.addLetra(input.getTyped());
	}


	void jogoUpdate() {
		vector<Conceito> inventario = jogo.getInventario();
		vector<string> paraEnviar;
		for (vector<Conceito>::iterator it = inventario.begin(); it != inventario.end(); it++)
			paraEnviar.push_back((*it).getNome());

		interfacer.setItens(paraEnviar);
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
	Overseer m = Overseer();
	input.add(&m, 0);
	jogo.add(&m, 1);

	m.main();
};