#include "mapa.h"
#include <queue>


Mapa::Mapa(vector<Sala> salasRecebidas) {
	// Gerar salas
	for (int i = 0; i < salasRecebidas.size(); i++) {
		Node novo = Node(salasRecebidas[i]);
		salas.push_back(novo);
	}

	// Popular nós com anexasIndex
	for (int i = 0; i < salas.size(); i++)
		// Colocar o index de cada sala anexa no Node
		for (int j = 0; j < salas[i].getElemento().getSalaAnexaCount(); j++)
			for (int k = 0; k < salas.size(); k++)
				if (salas[k].getElemento().getName() == salas[i].getElemento().getSalaAnexaNome(j)) {
					salas[i].addAnexa(k);
					break;
				}
}


vector<Sala> Mapa::optimalPath(Node salaOrigem, Node salaDestino) {
	vector<Sala> caminho;
	queue<Node> salasEsquerdas;
	queue<Node> salasDireitas;
	salasEsquerdas.push(salaOrigem);
	salasDireitas.push(salaDestino);

	while (!(salasEsquerdas.empty() || salasDireitas.empty())) {
		// Esquerda
		Node salaExaminada = salasEsquerdas.front();
		if (salaExaminada.getVisitado() == 2) {

			break;
		} else if (!salaExaminada.getVisitado()) {
			salaExaminada.setVisitado(1);
			for (int i = 0; i < salaExaminada.getAnexas().size(); i++)
				salasEsquerdas.push(salas[salaExaminada.getAnexas()[i]]);
		}
		salasEsquerdas.pop();

		// Direita
		salaExaminada = salasDireitas.front();
		if (salaExaminada.getVisitado() == 1) {
			break;
		}
		else if (salaExaminada.getVisitado()) {
			salaExaminada.setVisitado(2);
			for (int i = 0; i < salaExaminada.getAnexas().size(); i++)
				salasDireitas.push(salas[salaExaminada.getAnexas()[i]]);
		}
		salasDireitas.pop();
	}

	return caminho;
}

bool Mapa::existeSala(string name) {
	for (int i = 0; i < salas.size(); i++)
		if (salas[i].getElemento().getName() == name)
			return true;
	return false;
}


Sala Mapa::getSala(int index) { 
	return salas[index].getElemento();
}

Sala Mapa::getSala(string name) {
	for (int i = 0; i < salas.size(); i++)
		if (salas[i].getElemento().getName() == name)
			return getSala(i);
}