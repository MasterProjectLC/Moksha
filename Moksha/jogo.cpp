#include "jogo.h"

Jogo::Jogo() {
	FileManager fileManager = FileManager();
	
	vector<string> salaLista = fileManager.getFileList("files/salas");

	// Passe 1 - Gerar objetos de salas e colocá-los no vetor salas
	for (int i = 0; i < salaLista.size(); i++) {
		vector<string> salaLinhas = splitString(fileManager.readFromFile("files/salas/" + salaLista[i]), '\n');
		vector<string> salasAnexasNomes = splitString(salaLinhas[5], ',');
		Sala novaSala = Sala(salaLinhas[1], salaLinhas[3], salasAnexasNomes);
		salas.push_back(novaSala);
	}

	// Passe 2 - popular salasAnexas de cada objeto
	for (int i = 0; i < salas.size(); i++) {
		vector<int> salasAnexas;

		for (int j = 0; j < salas[i].getSalaAnexaCount(); j++) {
			for (int k = 0; k < salas.size(); k++) {
				if (salas[i].getSalaAnexaNome(j).compare(salas[k].getName()) == 0) {
					salasAnexas.push_back(k);
				}
			}
		}

		salas[i].setupAnexas(salasAnexas);
	}

	salaAtual = salas[2];
	
}

void Jogo::imprimirTexto(string texto) {
	this->texto = texto;
	setNotifyID(imprimir);
	notify();
}

string Jogo::concatenarArgs(vector<string> args, int inicio) {
	string r = args.at(inicio);
	for (int i = inicio+1; i < args.size(); i++) {
		r += " " + args.at(i);
	}
	return r;
}


vector<Conceito> Jogo::getInventario() {
	return inventario.getInventario();
}


void Jogo::receberArgs(vector<string> args) {
	if (args.size() >= 2) {
		if (args.at(0) == "obter") {
			inventario.addConceito(concatenarArgs(args, 1));
			setNotifyID(obter);
			notify();
		}

		else if (args.at(0) == "mover") {
			if (salaAtual.isSalaAnexa(concatenarArgs(args, 1))) {
				salaAtual = salas[salaAtual.getSalaAnexaIndex(concatenarArgs(args, 1))];

				imprimirTexto("Sala atual: " + salaAtual.getName() + "\n" + salaAtual.getTextoInicial());
				for (int i = 0; i < salaAtual.getSalaAnexaCount(); i++)
					imprimirTexto(salas[salaAtual.getSalaAnexaIndex(i)].getName());
			}
			else {
				imprimirTexto("Sala especificada nao e anexa a sala atual.");
			}
		}
	}
}