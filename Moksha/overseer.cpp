#include "interface.h"
#include "jogo.h"
//#include "fileManager.h"
#include <conio.h>
#include <ctype.h>

Interface interfacer = Interface(140, 40, 60, 30);
//FileManager fileManager = FileManager();
Jogo jogo = Jogo();

class Overseer : public IObserver {
public:
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
		case interfacer.notifyArgs:
			jogo.receberArgs(interfacer.getArgs());
		}
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
		interfacer.printLinha("Bem-Vindo a Medusa, Sr. Colt. Deixe-me trazer seu jaleco...");

		while (1) {
			interfacer.clocking();
		}

		return 0;
	}
};

int main() {
	Overseer m = Overseer();
	interfacer.add(&m, 0);
	jogo.add(&m, 1);

	m.main();
};