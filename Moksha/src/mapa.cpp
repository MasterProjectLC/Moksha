#include "mapa.h"

// GERAÇÃO E INICIALIZAÇÃO ----------------------------------------------------------

Mapa::Mapa(vector<Sala*> salasRecebidas, IObserver *observer) {
	this->observer = observer;

	// Gerar salas
	for (int i = 0; i < salasRecebidas.size(); i++) {
		Node novo = Node(salasRecebidas[i]);
		salas.push_back(novo);
	}

	// Popular nós com anexasIndex
	for (int i = 0; i < salas.size(); i++) {
		// Colocar o index de cada sala anexa no Node
		for (int j = 0; j < salas[i].getElemento()->getSalaAnexaCount(); j++)
			for (int k = 0; k < salas.size(); k++)
				if (salas[i].getElemento()->getSalaAnexaNome(j) == salas[k].getElemento()->getNome()) {
					salas[i].addAnexa(k);
					break;
				}

		carregarSala(salasRecebidas[i]);
	}
};


void Mapa::carregarSala(Sala *sala) {
	vector<string> objetoLista = FileManager::getFileList("files/objects");
	vector<string> objectNames = sala->getObjectNames();

	// Procurar objetos na lista
	for (int i = 0; i < objetoLista.size(); i++) {
		FileDict fileObject = FileManager::readFromFile(objetoLista[i]);

		for (int j = 0; j < objectNames.size(); j++) {
			// Encontrado - Gerar objeto
			if (objectNames[j].compare(fileObject.getValue("name")) == 0) {
				Object newObject = Object(fileObject);								// Gerar objeto
				newObject.add(observer, j + sala->getIndex()*MAX_OBJECT_COUNT);		// Gerar id de callcard
				sala->addObject(newObject);											// Adicionar objeto à sala
				break;
			}
		}
	}
}


Object* Mapa::getObject(int id) {
	return salas[id / MAX_OBJECT_COUNT].getElemento()->getObject(id % MAX_OBJECT_COUNT);
}

// CAMINHOS E PATHFINDING -----------------------------------------------------------------------

queue<Sala*> Mapa::optimalPath(Sala *_salaOrigem, Sala *_salaDestino) {
	queue<Sala*> caminho;
	limparVisitado();

	// Edge case
	if (_salaOrigem == _salaDestino)
		return caminho;

	// Encontra node da sala de origem
	queue<Node*> procura;
	stack<Node*> reverser;
	for (int i = 0; i < salas.size(); i++) {
		if (salas[i].getElemento()->getNome() == _salaOrigem->getNome()) {
			salas[i].setVisitado(1);
			procura.push(&salas[i]);
			break;
		}
	}

	// Iterar pelos nodes anexos à sala examinada
	while (!procura.empty()) {
		Node* salaExaminada = procura.front();
		for (int i = 0; i < salaExaminada->getAnexas().size(); i++) {
			// Não examinada ainda - adicionar à lista de checagem e colocar node atual como pai
			if (salas[salaExaminada->getAnexas()[i]].getVisitado() == 0) {
				procura.push(&salas[salaExaminada->getAnexas()[i]]);			// Coloca na lista
				salas[salaExaminada->getAnexas()[i]].setPai(salaExaminada);		// Seta o node atual como pai
				salas[salaExaminada->getAnexas()[i]].setVisitado(1);			// Seta visitado para 1

				// Sala alvo encontrada!
				if (salas[salaExaminada->getAnexas()[i]].getElemento() == _salaDestino) {
					reverser.push(&salas[salaExaminada->getAnexas()[i]]);

					// Construir caminho no reverso
					while (reverser.top()->temPai())
						reverser.push(reverser.top()->getPai());
					while (!reverser.empty()) {
						caminho.push(reverser.top()->getElemento());
						reverser.pop();
					}

					caminho.pop();	// Retira o primeiro da lista (a própria origem)
					return caminho;
				}
			}
		}
		procura.pop();
	}

	return caminho;
};


queue<Sala*> Mapa::breadthSearch(Sala *salaOrigem) {
	queue<Sala*> retorno;
	stack<int> st;
	limparVisitado(); // Reseta visitado

	// Procura pela salaOrigem no mapa de nós, e começa busca por ela
	for (int i = 0; i < salas.size(); i++) {
		if (salas[i].getElemento()->getNome() == salaOrigem->getNome()) {
			breadthSearchHelper(&retorno, st, i);
			break;
		}
	}

	retorno.pop(); // Remove a sala de origem da lista
	return retorno;
}


void Mapa::breadthSearchHelper(queue<Sala*> *retorno, stack<int> &st, int salaChecada) {
	if (!st.empty())
		st.pop(); // Retira da lista de processamento

	if (salas[salaChecada].getVisitado() == 0) {
		retorno->push(salas[salaChecada].getElemento()); // Adiciona à lista de retorno

		// Seta como visitado
		salas[salaChecada].setVisitado(1);

		// Coloca todos os nós vizinhos na lista de processamento
		for (int i = 0; i < salas[salaChecada].getAnexas().size(); i++) {
			st.push(salas[salaChecada].getAnexas()[i]);
		}
	}

	// Continua processamento
	if (!st.empty())
		breadthSearchHelper(retorno, st, st.top());
}


// GETTERS E HELPERS -----------------------------------------------------

bool Mapa::existeSala(string name) {
	for (int i = 0; i < salas.size(); i++)
		if (salas[i].getElemento()->getNome() == name)
			return true;
	return false;
};

Sala* Mapa::getSala(int index) { 
	return salas[index].getElemento();
};

Sala* Mapa::getSala(string name) {
	for (int i = 0; i < salas.size(); i++)
		if (salas[i].getElemento()->getNome() == name)
			return getSala(i);

	throw invalid_argument("There's no room with that name :(");
};

void Mapa::limparVisitado() {
	for (int i = 0; i < salas.size(); i++) {
		salas[i].setVisitado(0);
		salas[i].setPai(&salas[i]);
	}
}