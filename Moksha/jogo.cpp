#include "jogo.h"

Jogo::Jogo() {
	gerarMapa();

	jogador = Jogador();
	jogador.setSalaAtual(mapa.getSala("Corredor"));
	jogador.add(this, OBSERVER_OFFSET);
	personagens.push_back(&jogador);

	jenna = Jenna(&mapa);
	jenna.setSalaAtual(mapa.getSala("Banheiro"));
	jenna.setSalaAlvo(mapa.getSala("Corredor"));
	jenna.add(this, OBSERVER_OFFSET+1);
	personagens.push_back(&jenna);

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
			personagem->verSala(getPessoasNaSala(personagem->getSalaAtual()) );
			if (personagem->getNome() == jogador.getNome()) {
				advanceTime();
			}
			else {
				if (personagem->getSalaAtual() == jogador.getSalaAtual())
					imprimirTexto(personagem->getNome() + " entrou nesta sala.");
			}
			break;

		case personagem->mencionar:
			for (int i = 0; i < personagens.size(); i++) {
				if (personagens[i]->getNome() == personagem->getNotifyTargets()[0] 
							&& personagens[i]->getSalaAtual() == personagem->getSalaAtual()) {
					personagens[i]->executarReacao(personagem->getNotifyText(), personagem->getNome());
					if (id == 0)
						advanceTime();
					break;
				}
			}
			break;

		case personagem->falar:
			for (int i = 0; i < personagens.size(); i++) {
				if	(personagens[i]->getNome() == personagem->getNotifyTargets()[0]
							&& personagens[i]->getSalaAtual() == personagem->getSalaAtual()) {
					personagens[i]->executarReacao(personagem->getNotifyText(), personagem->getNome());
					if (id == 0)
						advanceTime();
					break;
				}
			}
			break;


		default:
			return;
		}
		
	}
}


void Jogo::advanceTime() {
	// TODO: create a separate function for this
	if (jenna.getSalaAtual() != jogador.getSalaAtual())
		jenna.takeAction();
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


vector<string> Jogo::getPessoasNaSala(Sala* sala) {
	vector<string> retorno;
	for (int i = 0; i < personagens.size(); i++) {
		if (personagens[i]->getSalaAtual() == sala)
			retorno.push_back(personagens[i]->getNome());
	}

	return retorno;
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