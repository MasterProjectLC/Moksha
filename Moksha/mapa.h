#pragma once
#pragma once
#include <vector>
#include <string>
#include "sala.h"

using namespace std;

class Mapa {
private:
	class Node {
	private:
		Sala elemento;
		int visitado;
		vector<int> anexasIndex;

	public:
		Node(Sala elemento) {
			this->visitado = 0;
			this->elemento = elemento;
		}

		int getVisitado() { return visitado; }
		void setVisitado(int v) { visitado = v; }

		Sala getElemento() { return elemento; }

		void addAnexa(int novo) { anexasIndex.push_back(novo); };
		vector<int> getAnexas() { return anexasIndex; };
	};

	// Atributos
	vector<Node> salas;
	vector<Objeto> objetos;

	vector<Sala> optimalPath(Node salaOrigem, Node salaDestino);

public:
	Mapa() {};

	Mapa(vector<Sala> salasRecebidas);

	bool existeSala(string name);
	Sala getSala(int index);
	Sala getSala(string name);
};