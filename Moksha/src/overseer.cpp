#include "interface.h"
#include "game.h"
#include "saveModule.h"
#include <conio.h>
#include <ctype.h>

class Overseer : public IObserver {
private:
	Interface interfacer = Interface(140, 40, 60, 30);
	Game game = Game();
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
		interfacer.mainInterface();
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
			interfacer.printLine(game.getText());
		}

		else if (game.getNotifyID() == game._obter) {
			vector<Item*> inventory = game.getItems();
			vector<string> paraEnviar;
			for (int i = 0; i < inventory.size(); i++)
				paraEnviar.push_back(inventory[i]->getName());

			interfacer.setItems(paraEnviar, interfacer.item);
		}

		else if (game.getNotifyID() == game._ouvir) {
			vector<Concept*> inventory = game.getConcepts();
			vector<string> paraEnviar;
			for (int i = 0; i < inventory.size(); i++)
				paraEnviar.push_back(inventory[i]->getName());

			interfacer.setItems(paraEnviar, interfacer.rumor);
		}
	}


	int main() {
		interfacer.printLine("Welcome to Medusa, Mr. Elliot. Let me take your coat...");

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