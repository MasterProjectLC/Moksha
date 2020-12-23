#include "jogo.h"

Jogo::Jogo() {
	fileManager = FileManager();
	FileDict fileErros = fileManager.readFromFile("files/erros.txt");
	erroSemObjeto = fileErros.getValues("sem objeto")[0];
	erroSemAcao = fileErros.getValues("sem acao")[0];
	erroSemSala = fileErros.getValues("sem sala")[0];

	gerarMapa();
	jogador.setSalaAtual(mapa.getSala(1));
	carregarSala(jogador.getSalaAtual());
}


void Jogo::update(int id) {
	Objeto objeto = getSalaAtual()->getObjeto(id);
	
	// Obter
	if (objeto.getNotifyID() == objeto.obter) {
		addItem(objeto.getName());

		Sala* sala = jogador.getSalaAtual();
		sala->removeObjeto(objeto);
	}
}


void Jogo::receberArgs(vector<string> args) {
	if (args.size() >= 2 && jogador.isAcaoValida(args.at(0))) {
		string secondArg = concatStrings(args, 1);

		// Mover para outra sala
		if (args.at(0) == "mover") {
			if (getSalaAtual()->isSalaAnexa(secondArg))
				jogador.setSalaAtual(moverSala(*getSalaAtual(), secondArg));
			else
				imprimirTexto(erroSemSala);
		}

		// Objetos
		else {
			if (getSalaAtual()->possuiObjeto(secondArg)) {
				Objeto objetoAqui = getSalaAtual()->getObjeto(secondArg);
				objetoAqui.takeAction(args.at(0));

				// Imprimir resposta
				vector<string> responses = objetoAqui.getResponses(args.at(0));
				if (responses.size() > 0) { imprimirTexto(responses[0]); }
				else { imprimirTexto(erroSemAcao); }
			}

			// Objeto não existe
			else
				imprimirTexto(erroSemObjeto);
		}
	}
}

// LIDAR COM MAPA ------------------------------------------

void Jogo::gerarMapa() {
	vector<string> salaLista = fileManager.getFileList("files/salas");

	// Gerar salas e colocá-los no Mapa
	vector<Sala> salas;
	for (int i = 0; i < salaLista.size(); i++) {
		FileDict fileSala = fileManager.readFromFile(salaLista[i]);
		salas.push_back(Sala(fileSala.getValue("nome"), fileSala.getValue("texto"),
							fileSala.getValues("adjacentes"), fileSala.getValues("objetos")));
	}

	mapa = Mapa(salas);
}


void Jogo::carregarSala(Sala *sala) {
	vector<string> objetoLista = fileManager.getFileList("files/objetos");
	vector<string> objetoNomes = sala->getObjetoNomes();

	// Procurar objetos na lista
	for (int i = 0; i < objetoLista.size(); i++) {
		FileDict fileObjeto = fileManager.readFromFile(objetoLista[i]);

		for (int j = 0; j < objetoNomes.size(); j++) {
			// Encontrado
			if (objetoNomes[j].compare(fileObjeto.getValue("nome")) == 0) {
				vector<vector<string>> objetoActions;
				vector<vector<string>> objetoResponses;
				vector<vector<string>> objetoCombos = fileObjeto.getKeys();
				
				// Cada combo acao-resposta possivel
				for (int i = 0; i < objetoCombos.size(); i++) {
					if (objetoCombos[i][0] == "nome" || objetoCombos[i][0] == "acoes")
						continue;
					objetoActions.push_back(objetoCombos[i]);
					objetoResponses.push_back(fileObjeto.getValues(objetoCombos[i][0]));
				}

				Objeto newObjeto = Objeto(fileObjeto.getValue("nome"), fileObjeto.getValues("acoes"), objetoActions, objetoResponses);
				newObjeto.add(this, j);
				sala->addObjeto(newObjeto);
				break;
			}
		}
	}
}

// ACTIONS ----------------------------------

void Jogo::addItem(string item) {
	jogador.addItem(item);
	notify(obter);
}

Sala Jogo::moverSala(Sala salaOrigem, string salaDestino) {
	// Movimento
	salaOrigem = mapa.getSala(salaDestino);
	carregarSala(&salaOrigem);

	// Mensagens
	imprimirTexto("Sala atual: " + salaOrigem.getName() + "\n" + salaOrigem.getTextoInicial() + "\nSalas anexas:");
	for (int i = 0; i < salaOrigem.getSalaAnexaCount(); i++)
		imprimirTexto(salaOrigem.getSalaAnexaNome(i));

	imprimirTexto("Objetos na sala: ");
	vector<Objeto> objetos = salaOrigem.getObjetos();
	for (int i = 0; i < objetos.size(); i++) {
		imprimirTexto(objetos[i].getName());
	}

	return salaOrigem;
}


void Jogo::imprimirTexto(string texto) {
	this->texto = texto;
	notify(imprimir);
}


vector<Item> Jogo::getInventario() {
	return jogador.getInventario();
}