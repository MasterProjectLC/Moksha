#include "jogo.h"

Jogo::Jogo() {
	gerarMapa();

	jogador = Jogador();
	jogador.setSalaAtual(mapa.getSala("Corredor"));
	jogador.add(this, OBSERVER_OFFSET);
	personagens[0] = &jogador;

	jenna = Jenna(&mapa);
	jenna.setSalaAtual(mapa.getSala("Banheiro"));
	jenna.setSalaAlvo(mapa.getSala("Corredor"));
	jenna.add(this, OBSERVER_OFFSET+1);
	personagens[1] = &jenna;

	mapa.carregarSala(jogador.getSalaAtual());
}


void Jogo::update(int id) {
	// Objeto
	if (id < OBSERVER_OFFSET) {
		Objeto objeto = getSalaAtual()->getObjeto(id);

		// Obter
		if (objeto.getNotifyID() == objeto.obter) {
			addItem(objeto.getName());

			Sala* sala = jogador.getSalaAtual();
			sala->removeObjeto(objeto);
		}
	}

	// Personagem
	else {
		id -= OBSERVER_OFFSET;
		Personagem* personagem = personagens[id];

		switch (personagem->getNotifyID()) {
		case personagem->imprimir:
			imprimirTexto(personagem->getNotifyText());
			break;
		case personagem->mover:
			personagem->setSalaAtual(moverSala(personagem->getSalaAtual(), personagem->getNotifyText()));
			if (id == 0) {
				// Mensagens (TODO: this is dumb, fix it later)
				Sala* salaOrigem = personagem->getSalaAtual();
				imprimirTexto("Sala atual: " + salaOrigem->getName() + "\n" + salaOrigem->getTextoInicial() + "\nSalas anexas:");
				for (int i = 0; i < salaOrigem->getSalaAnexaCount(); i++)
					imprimirTexto(salaOrigem->getSalaAnexaNome(i));

				imprimirTexto("Objetos na sala: ");
				vector<Objeto> objetos = salaOrigem->getObjetos();
				for (int i = 0; i < objetos.size(); i++) {
					imprimirTexto(objetos[i].getName());
				}
			}
			break;
		default:
			return;
		}
		
		// Player moved. Update everyone
		if (id == 0) {
			// TODO: create a separate function for this
			jenna.takeAction();
			imprimirTexto("Sala da Jenna: " + jenna.getSalaAtual()->getName());
		}
	}
}


void Jogo::receberArgs(vector<string> args) {
	jogador.receberArgs(args);
}

// LIDAR COM MAPA ------------------------------------------

void Jogo::gerarMapa() {
	vector<string> salaLista = FileManager::getFileList("files/salas");

	// Gerar salas e colocá-los no Mapa
	vector<Sala*> salas;
	for (int i = 0; i < salaLista.size(); i++) {
		FileDict fileSala = FileManager::readFromFile(salaLista[i]);
		salas.push_back(new Sala(fileSala.getValue("nome"), fileSala.getValue("texto"),
							fileSala.getValues("adjacentes"), fileSala.getValues("objetos")));
	}

	mapa = Mapa(salas, this);
}


// ACTIONS ----------------------------------

void Jogo::addItem(string item) {
	jogador.addItem(item);
	notify(obter);
}

Sala* Jogo::moverSala(Sala* salaOrigem, string salaDestino) {
	// Movimento
	salaOrigem = mapa.getSala(salaDestino);
	mapa.carregarSala(salaOrigem);

	return salaOrigem;
}


void Jogo::imprimirTexto(string texto) {
	this->texto = texto;
	notify(imprimir);
}


vector<Item> Jogo::getInventario() {
	return jogador.getInventario();
}