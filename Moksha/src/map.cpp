#include "map.h"

// GERAÇÃO E INICIALIZAÇÃO ----------------------------------------------------------

Map::Map(vector<Room*> salasRecebidas, IObserver *observer) {
	this->observer = observer;

	// Gerar salas
	for (int i = 0; i < salasRecebidas.size(); i++) {
		Node novo = Node(salasRecebidas[i]);
		rooms.push_back(novo);
	}

	// Popular nós com anexasIndex
	for (int i = 0; i < rooms.size(); i++) {
		// Colocar o index de cada sala anexa no Node
		for (int j = 0; j < rooms[i].getElement()->getAdjacentRoomCount(); j++)
			for (int k = 0; k < rooms.size(); k++)
				if (rooms[i].getElement()->getAdjacentRoomName(j) == rooms[k].getElement()->getName()) {
					rooms[i].addAdjacent(k);
					break;
				}

		loadRoom(salasRecebidas[i]);
	}
};


void Map::loadRoom(Room *room) {
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


Object* Map::getObject(int id) {
	return rooms[id / MAX_OBJECT_COUNT].getElement()->getObject(id % MAX_OBJECT_COUNT);
}

// CAMINHOS E PATHFINDING -----------------------------------------------------------------------

queue<Room*> Map::optimalPath(Room *origin, Room *destination) {
	queue<Room*> caminho;
	clearVisited();

	// Edge case
	if (origin == destination)
		return caminho;

	// Encontra node da sala de origem
	queue<Node*> procura;
	stack<Node*> reverser;
	for (int i = 0; i < rooms.size(); i++) {
		if (rooms[i].getElement()->getName() == origin->getName()) {
			rooms[i].setVisited(1);
			procura.push(&rooms[i]);
			break;
		}
	}

	// Iterar pelos nodes anexos à sala examinada
	while (!procura.empty()) {
		Node* checkedRoom = procura.front();
		for (int i = 0; i < checkedRoom->getAdjacents().size(); i++) {
			// Não examinada ainda - adicionar à lista de checagem e colocar node atual como pai
			if (rooms[checkedRoom->getAdjacents()[i]].getVisited() == 0) {
				procura.push(&rooms[checkedRoom->getAdjacents()[i]]);			// Coloca na lista
				rooms[checkedRoom->getAdjacents()[i]].setParent(checkedRoom);		// Seta o node atual como pai
				rooms[checkedRoom->getAdjacents()[i]].setVisited(1);			// Seta visitado para 1

				// Sala alvo encontrada!
				if (rooms[checkedRoom->getAdjacents()[i]].getElement() == destination) {
					reverser.push(&rooms[checkedRoom->getAdjacents()[i]]);

					// Construir caminho no reverso
					while (reverser.top()->hasParent())
						reverser.push(reverser.top()->getParent());
					while (!reverser.empty()) {
						caminho.push(reverser.top()->getElement());
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


queue<Room*> Map::breadthSearch(Room *origin) {
	queue<Room*> retorno;
	stack<int> st;
	clearVisited(); // Reseta visitado

	// Procura pela salaOrigem no mapa de nós, e começa busca por ela
	for (int i = 0; i < rooms.size(); i++) {
		if (rooms[i].getElement()->getName() == origin->getName()) {
			breadthSearchHelper(&retorno, st, i);
			break;
		}
	}

	retorno.pop(); // Remove a sala de origem da lista
	return retorno;
}


void Map::breadthSearchHelper(queue<Room*> *retorno, stack<int> &st, int salaChecada) {
	if (!st.empty())
		st.pop(); // Retira da lista de processamento

	if (rooms[salaChecada].getVisited() == 0) {
		retorno->push(rooms[salaChecada].getElement()); // Adiciona à lista de retorno

		// Seta como visitado
		rooms[salaChecada].setVisited(1);

		// Coloca todos os nós vizinhos na lista de processamento
		for (int i = 0; i < rooms[salaChecada].getAdjacents().size(); i++) {
			st.push(rooms[salaChecada].getAdjacents()[i]);
		}
	}

	// Continua processamento
	if (!st.empty())
		breadthSearchHelper(retorno, st, st.top());
}


// GETTERS E HELPERS -----------------------------------------------------

bool Map::hasRoom(string name) {
	for (int i = 0; i < rooms.size(); i++)
		if (rooms[i].getElement()->getName() == name)
			return true;
	return false;
};

Room* Map::getRoom(int index) {
	return rooms[index].getElement();
};

Room* Map::getRoom(string name) {
	for (int i = 0; i < rooms.size(); i++)
		if (rooms[i].getElement()->getName() == name)
			return getRoom(i);

	throw invalid_argument("There's no room with that name :(");
};

void Map::clearVisited() {
	for (int i = 0; i < rooms.size(); i++) {
		rooms[i].setVisited(0);
		rooms[i].setParent(&rooms[i]);
	}
}