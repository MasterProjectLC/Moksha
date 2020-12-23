#include "interface.h"
#include "jogo.h"
#include <conio.h>
#include <ctype.h>

class Overseer : public IObserver {
private:
	Jogo jogo = Jogo();
	Interface interfacer = Interface(140, 40, 60, 30);
public:
	Overseer() {
		interfacer.add(this, 0);
		jogo.add(this, 1);
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
			jogo.receberArgs(interfacer.getArgs());
			break;
		}
	}


	void jogoUpdate() {
		if (jogo.getNotifyID() == jogo.imprimir) {
			interfacer.printLinha(jogo.getTexto());
		}

		else if (jogo.getNotifyID() == jogo.obter) {
			vector<Item> inventario = jogo.getInventario();
			vector<string> paraEnviar;
			for (vector<Item>::iterator it = inventario.begin(); it != inventario.end(); it++)
				paraEnviar.push_back((*it).getNome());

			interfacer.setItens(paraEnviar);

		}
	}


	int main() {
		interfacer.printLinha("Bem-Vindo a Medusa, Sr. Colt. Deixe-me trazer seu jaleco...");

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