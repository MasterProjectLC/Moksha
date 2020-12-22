#include "jogo.h"

Jogo::Jogo() {
	fileManager = FileManager();
	FileDict fileErros = fileManager.readFromFile("files/erros.txt");
	erroSemObjeto = fileErros.getValues("sem objeto")[0];
	erroSemAcao = fileErros.getValues("sem acao")[0];

	generateSalas();
	salaAtual = salas[1];
	carregarSala(&salaAtual);
}


void Jogo::update(int id) {
	int nid = objetos[id].getNotifyID();
	
	// Obter
	if (nid == objetos[id].obter) {
		addItem(objetos[id].getName());
		objetos.erase(objetos.begin()+id);
	}
}


void Jogo::imprimirTexto(string texto) {
	this->texto = texto;
	notify(imprimir);
}


vector<Item> Jogo::getInventario() {
	return jogador.getInventario();
}


void Jogo::receberArgs(vector<string> args) {
	if (args.size() >= 2 && jogador.isAcaoValida(args.at(0))) {
		// Mover para outra sala
		if (args.at(0) == "mover") {
			if (salaAtual.isSalaAnexa(concatStrings(args, 1))) {
				moverSala(&salaAtual, concatStrings(args, 1));
			}
			else {
				imprimirTexto("Sala especificada nao e anexa a sala atual.");
			}
		}

		// Objetos
		else {
			vector<Objeto> objetosAqui = salaAtual.getObjetos();
			bool objetoExiste = false;
			// Procurar objeto
			for (int i = 0; i < objetosAqui.size(); i++) {
				// Objeto encontrado
				if (args.at(1).compare(objetosAqui[i].getName()) == 0) {
					// Tomar ação
					objetosAqui[i].takeAction(args.at(0));

					// Imprimir resposta
					vector<string> responses = objetosAqui[i].getResponses(args.at(0));
					if (responses.size() > 0) { imprimirTexto(responses[0]); }
					else { imprimirTexto(erroSemAcao); }
					objetoExiste = true;
					break;
				}
			}
			
			// Objeto não existe
			if (!objetoExiste)
				imprimirTexto(erroSemObjeto);
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

				Objeto newObjeto = Objeto(fileObjeto.getValue("nome"), fileObjeto.getValues("acoes"), objetoActions, objetoResponses);
				newObjeto.add(this, objetos.size());
				objetos.push_back(newObjeto);
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

void Jogo::moverSala(Sala* salaOrigem, string salaDestino) {
	// Limpeza
	objetos.clear();

	// Movimento
	*salaOrigem = salas[salaAtual.getSalaAnexaIndex(salaDestino)];
	carregarSala(salaOrigem);

	// Mensagens
	imprimirTexto("Sala atual: " + salaOrigem->getName() + "\n" + salaOrigem->getTextoInicial());
	for (int i = 0; i < salaOrigem->getSalaAnexaCount(); i++)
		imprimirTexto(salas[salaOrigem->getSalaAnexaIndex(i)].getName());

	imprimirTexto("Objetos na sala: ");
	for (int i = 0; i < salaOrigem->getObjetos().size(); i++)
		imprimirTexto(salaOrigem->getObjetos()[i].getName());
}