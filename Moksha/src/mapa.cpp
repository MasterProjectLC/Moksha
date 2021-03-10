#include "mapa.h"

// GERAÇÃO E INICIALIZAÇÃO ----------------------------------------------------------

Mapa::Mapa(vector<Sala*> salasRecebidas, IObserver *observer) {
	this->observer = observer;

	// Gerar salas
	for (int i = 0; i < salasRecebidas.size(); i++) {
		Node novo = Node(salasRecebidas[i]);
		rooms.push_back(novo);
	}

	// Popular nós com anexasIndex
	for (int i = 0; i < rooms.size(); i++) {
		// Colocar o index de cada sala anexa no Node
		for (int j = 0; j < rooms[i].getElemento()->getSalaAnexaCount(); j++)
			for (int k = 0; k < rooms.size(); k++)
				if (rooms[i].getElemento()->getAdjacentRoomName(j) == rooms[k].getElemento()->getName()) {
					rooms[i].addAnexa(k);
					break;
				}

		carregarSala(salasRecebidas[i]);
	}
};


void Mapa::carregarSala(Sala *room) {
	vector<string> objectList = FileManager::getFileList("files/objects");
	vector<string> objectNames = room->getObjectNames();

	// Procurar objetos na lista
	for (int i = 0; i < objectList.size(); i++) {
		FileDict fileObject = FileManager::readFromFile(objectList[i]);

		for (int j = 0; j < objectNames.size(); j++) {
			// Encontrado - Gerar objeto
			if (objectNames[j].compare(fileObject.getValue("name")) == 0) {
				Object newObject = Object(fileObject);								// Gerar objeto
				newObject.add(observer, j + room->getIndex()*MAX_OBJECT_COUNT);		// Gerar id de callcard
				room->addObject(newObject);											// Adicionar objeto à sala
				break;
			}
		}
	}
}


Object* Mapa::getObject(int id) {
	return rooms[id / MAX_OBJECT_COUNT].getElemento()->getObject(id % MAX_OBJECT_COUNT);
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
	for (int i = 0; i < rooms.size(); i++) {
		if (rooms[i].getElemento()->getName() == _salaOrigem->getName()) {
			rooms[i].setVisitado(1);
			procura.push(&rooms[i]);
			break;
		}
	}

	// Iterar pelos nodes anexos à sala examinada
	while (!procura.empty()) {
		Node* salaExaminada = procura.front();
		for (int i = 0; i < salaExaminada->getAnexas().size(); i++) {
			// Não examinada ainda - adicionar à lista de checagem e colocar node atual como pai
			if (rooms[salaExaminada->getAnexas()[i]].getVisitado() == 0) {
				procura.push(&rooms[salaExaminada->getAnexas()[i]]);			// Coloca na lista
				rooms[salaExaminada->getAnexas()[i]].setPai(salaExaminada);		// Seta o node atual como pai
				rooms[salaExaminada->getAnexas()[i]].setVisitado(1);			// Seta visitado para 1

				// Sala alvo encontrada!
				if (rooms[salaExaminada->getAnexas()[i]].getElemento() == _salaDestino) {
					reverser.push(&rooms[salaExaminada->getAnexas()[i]]);

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
	for (int i = 0; i < rooms.size(); i++) {
		if (rooms[i].getElemento()->getName() == salaOrigem->getName()) {
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

	if (rooms[salaChecada].getVisitado() == 0) {
		retorno->push(rooms[salaChecada].getElemento()); // Adiciona à lista de retorno

		// Seta como visitado
		rooms[salaChecada].setVisitado(1);

		// Coloca todos os nós vizinhos na lista de processamento
		for (int i = 0; i < rooms[salaChecada].getAnexas().size(); i++) {
			st.push(rooms[salaChecada].getAnexas()[i]);
		}
	}

	// Continua processamento
	if (!st.empty())
		breadthSearchHelper(retorno, st, st.top());
}


// GETTERS E HELPERS -----------------------------------------------------

bool Mapa::hasRoom(string name) {
	for (int i = 0; i < rooms.size(); i++)
		if (rooms[i].getElemento()->getName() == name)
			return true;
	return false;
};

Sala* Mapa::getRoom(int index) {
	return rooms[index].getElemento();
};

Sala* Mapa::getRoom(string name) {
	for (int i = 0; i < rooms.size(); i++)
		if (rooms[i].getElemento()->getName() == name)
			return getRoom(i);

	throw invalid_argument("There's no room with that name :(");
};

void Mapa::limparVisitado() {
	for (int i = 0; i < rooms.size(); i++) {
		rooms[i].setVisitado(0);
		rooms[i].setPai(&rooms[i]);
	}
}