#include "mapa.h"


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
	vector<string> objetoLista = FileManager::getFileList("files/objetos");
	vector<string> objetoNomes = sala->getObjetoNomes();

	// Procurar objetos na lista
	for (int i = 0; i < objetoLista.size(); i++) {
		FileDict fileObjeto = FileManager::readFromFile(objetoLista[i]);

		for (int j = 0; j < objetoNomes.size(); j++) {
			// Encontrado - Gerar objeto
			if (objetoNomes[j].compare(fileObjeto.getValue("nome")) == 0) {
				Objeto newObjeto = Objeto(fileObjeto);
				newObjeto.add(observer, j);
				sala->addObjeto(newObjeto);
				break;
			}
		}
	}
}


queue<Sala*> Mapa::optimalPath(Sala *_salaOrigem, Sala *_salaDestino) {
	Node *salaOrigem, *salaDestino;

	for (int i = 0; i < salas.size(); i++) {
		if (salas[i].getElemento()->getNome() == _salaOrigem->getNome())
			salaOrigem = &salas[i];

		if (salas[i].getElemento()->getNome() == _salaDestino->getNome())
			salaDestino = &salas[i];
	}

	queue<Sala*> caminho;
	queue<Node*> salasEsquerdas; salasEsquerdas.push(salaOrigem);
	queue<Node*> salasDireitas; salasDireitas.push(salaDestino);
	vector<Node*> ponte;
	salaOrigem->setVisitado(1);
	salaDestino->setVisitado(2);

	while (!salasEsquerdas.empty() || !salasDireitas.empty()) {
		if (optimalPathHelper(&salasEsquerdas, false, &ponte) || optimalPathHelper(&salasDireitas, true, &ponte)) {
			// Caminho encontrado - criar vetor
			// Esquerda
			stack<Sala*> s;
			s.push(ponte.at(0)->getElemento());
			while (ponte.at(0)->temPai()) {
				ponte.emplace(ponte.begin(), ponte.at(0)->getPai());
				ponte.erase(ponte.begin()+1);
				s.push(ponte.at(0)->getElemento());
			}

			s.pop();
			while (!s.empty()) {
				caminho.push(s.top());
				s.pop();
			}

			// Direita
			caminho.push(ponte.at(1)->getElemento());
			while (ponte.at(1)->temPai()) {
				ponte.emplace(ponte.begin()+1, ponte.at(1)->getPai());
				ponte.erase(ponte.begin()+2);
				caminho.push(ponte.at(1)->getElemento());
			}
			break;
		}
	}

	limparVisitado();
	return caminho;
};


bool Mapa::optimalPathHelper(queue<Node*> *salasExaminadas, bool isDireita, vector<Node*> *ponte) {
	int visitado = 1 + (isDireita), alvo = 1 + (!isDireita);

	if (salasExaminadas->empty())
		return false;

	Node* salaExaminada = salasExaminadas->front();

	// Iterar pelos nodes anexos à sala examinada
	for (int i = 0; i < salaExaminada->getAnexas().size(); i++) {
		// Não examinada ainda - adicionar à lista
		if (salas[salaExaminada->getAnexas()[i]].getVisitado() == 0) {
			salasExaminadas->push(&salas[salaExaminada->getAnexas()[i]]);
			salas[salaExaminada->getAnexas()[i]].setPai(salaExaminada);
			salas[salaExaminada->getAnexas()[i]].setVisitado(visitado);
		}

		// Sala do outro lado encontrada! Retorná-la
		else if (salas[salaExaminada->getAnexas()[i]].getVisitado() == alvo) {
			if (!isDireita) {
				ponte->push_back(salaExaminada);
				ponte->push_back(&salas[salaExaminada->getAnexas()[i]]);
				return true;
			}
		}
	}

	salasExaminadas->pop();
	return false;
};


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
	}
}