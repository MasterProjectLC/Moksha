#include "jogo.h"

Jogo::Jogo() {
	fileManager = FileManager();
	
	generateSalas();
	salaAtual = salas[1];
	carregarSala(&salaAtual);
}


void Jogo::imprimirTexto(string texto) {
	this->texto = texto;
	setNotifyID(imprimir);
	notify();
}


vector<Conceito> Jogo::getInventario() {
	return inventario.getInventario();
}


void Jogo::receberArgs(vector<string> args) {
	if (args.size() >= 2) {
		// Obter item
		if (args.at(0) == "obter") {
			inventario.addConceito(concatStrings(args, 1));
			setNotifyID(obter);
			notify();
		}

		// Mover para outra sala
		else if (args.at(0) == "mover") {
			if (salaAtual.isSalaAnexa(concatStrings(args, 1))) {
				salaAtual = salas[salaAtual.getSalaAnexaIndex(concatStrings(args, 1))];
				carregarSala(&salaAtual);
				imprimirTexto("Sala atual: " + salaAtual.getName() + "\n" + salaAtual.getTextoInicial());
				for (int i = 0; i < salaAtual.getSalaAnexaCount(); i++)
					imprimirTexto(salas[salaAtual.getSalaAnexaIndex(i)].getName());

				imprimirTexto("Objetos na sala: ");
				for (int i = 0; i < salaAtual.getObjetos().size(); i++)
					imprimirTexto(salaAtual.getObjetos()[i].getName());
			}
			else {
				imprimirTexto("Sala especificada nao e anexa a sala atual.");
			}
		}

		// Objetos
		else {
			vector<Objeto> objetosAqui = salaAtual.getObjetos();
			for (int i = 0; i < objetosAqui.size(); i++) {
				if (args.at(1).compare(objetosAqui[i].getName()) == 0) {
					vector<string> responses = objetosAqui[i].getResponses(args.at(0));
					if (responses.size() > 0) { imprimirTexto(responses[0]); }
				}
			}
		}
	}
}

// LOADING & GENERATING ------------------------------------------

void Jogo::generateSalas() {
	vector<string> salaLista = fileManager.getFileList("files/salas");

	// Passe 1 - Gerar objetos de salas e colocá-los no vetor salas
	for (int i = 0; i < salaLista.size(); i++) {
		FileDict fileSala = fileManager.readFromFile(salaLista[i]);
		Sala novaSala = Sala(fileSala.getValue("nome"), fileSala.getValue("texto"), 
				fileSala.getValues("adjacentes"), fileSala.getValues("objetos"));
		salas.push_back(novaSala);
	}

	// Passe 2 - popular salasAnexas de cada objeto
	for (int i = 0; i < salas.size(); i++) {
		vector<int> salasAnexas;

		for (int j = 0; j < salas[i].getSalaAnexaCount(); j++) {
			for (int k = 0; k < salas.size(); k++) {
				cout << salas[i].getSalaAnexaNome(j) + " ? " + salas[k].getName() << endl;
				if (salas[i].getSalaAnexaNome(j).compare(salas[k].getName()) == 0) {

					salasAnexas.push_back(k);
				}
			}
		}
		salas[i].setupAnexas(salasAnexas);
	}
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

				sala->addObjeto(Objeto(fileObjeto.getValue("nome"), fileObjeto.getValues("acoes"), objetoActions, objetoResponses));
				break;
			}
		}
	}
}