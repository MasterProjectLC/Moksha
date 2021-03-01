#include "interface.h"
#include "jogo.h"
#include <conio.h>
#include <ctype.h>

class Overseer : public IObserver {
private:
	Interface interfacer = Interface(140, 40, 60, 30);
	Jogo game = Jogo();
public:
	Overseer() {
		interfacer.add(this, 0);
		game.add(this, 1);

		game.setup();
	}


	void update(int id) override {
		// Update
		switch (id) {
		case 0:
			interfaceUpdate();
			break;
		case 1:
			jogoUpdate();
			break;
		}

		// Design & Draw
		interfacer.interfacePrincipal();
	}


	void interfaceUpdate() {
		switch (interfacer.getNotifyID()) {
		case this->interfacer.notifyArgs:
			game.receiveArgs(interfacer.getArgs());
			break;
		}
	}


	void jogoUpdate() {
		if (game.getNotifyID() == game._imprimir) {
			interfacer.printLinha(game.getText());
		}

		else if (game.getNotifyID() == game._obter) {
			vector<Item> inventario = game.getInventory();
			vector<string> paraEnviar;
			for (int i = 0; i < inventario.size(); i++)
				paraEnviar.push_back(inventario[i].getNome());

			interfacer.setItens(paraEnviar);

		}
	}


	int main() {
		interfacer.printLinha("Welcome to Medusa, Mr. Elliot. Let me take your coat...");

		while (1) {
			interfacer.clocking();
		}

		return 0;
	}
};

int main() {
	Overseer m = Overseer();
	m.main();
};