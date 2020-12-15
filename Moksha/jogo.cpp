#include "jogo.h"

Jogo::Jogo() {
	fileManager = FileManager();
	
	generateSalas();
	salaAtual = salas[2];
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
				for (int i = 0; i < salaAtual.getObjetos().size(); i++) {
					imprimirTexto(salaAtual.getObjetos()[i].getName());
				}
			}
			else {
				imprimirTexto("Sala especificada nao e anexa a sala atual.");
			}
		}

		// Objetos
		else {
			vector<Objeto> objetosAqui = salaAtual.getObjetos();
			for (int i = 0; i < objetosAqui.size(); i++) {
				vector<string> responses = objetosAqui[i].getResponses(args.at(0));
				if (responses.size() > 0) { imprimirTexto(responses[0]); }
			}
		}
	}
}

// LOADING & GENERATING ------------------------------------------

void Jogo::generateSalas() {
	vector<string> salaLista = fileManager.getFileList("files/salas");

	// Passe 1 - Gerar objetos de salas e colocá-los no vetor salas
	for (int i = 0; i < salaLista.size(); i++) {
		vector<string> salaLinhas = splitString(fileManager.readFromFile(salaLista[i]), '\n');
		Sala novaSala = Sala(salaLinhas[1], salaLinhas[3], splitString(salaLinhas[5], ','), splitString(salaLinhas[7], ','));
		salas.push_back(novaSala);
	}

	// Passe 2 - popular salasAnexas de cada objeto
	for (int i = 0; i < salas.size(); i++) {
		vector<int> salasAnexas;

		for (int j = 0; j < salas[i].getSalaAnexaCount(); j++) {
			for (int k = 0; k < salas.size(); k++) {
				if (salas[i].getSalaAnexaNome(j).compare(salas[k].getName()) == 0)
					salasAnexas.push_back(k);
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
		vector<string> objetoLinhas = splitString(fileManager.readFromFile(objetoLista[i]), '\n');

		for (int j = 0; j < objetoNomes.size(); j++) {
			// Encontrado
			if (objetoNomes[j].compare(objetoLinhas[1]) == 0) {
				vector<string> objetoValidActions = splitString(objetoLinhas[3], ',');
				vector<vector<string>> objetoActions;
				vector<vector<string>> objetoResponses;

				// Cada combo acao-resposta possivel
				for (int i = 4; i+1 < objetoLinhas.size(); i += 2) {
					objetoActions.push_back(splitString(objetoLinhas[i].substr(0, objetoLinhas[i].length()-1), ','));
					objetoResponses.push_back(splitString(objetoLinhas[i + 1], '\n'));
				}

				sala->addObjeto(Objeto(objetoLinhas[1], objetoValidActions, objetoActions, objetoResponses));
				break;
			}
		}
	}
}