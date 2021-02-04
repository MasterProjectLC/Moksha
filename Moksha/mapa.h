#pragma once
#pragma once
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include "fileManager.h"
#include "sala.h"
#include "dictionary.h"
#include "IObserver.h"

using namespace std;

class Mapa {
private:
	class Node {
	private:
		Sala *elemento;
		int visitado;
		vector<int> anexasIndex;
		Node* paiIndex;
		bool temPai_;

	public:
		Node() {};

		Node(Sala *elemento) {
			this->visitado = 0;
			this->elemento = elemento;
			this->temPai_ = false;
		};

		int getVisitado() { return visitado; };
		void setVisitado(int v) { visitado = v; };

		bool temPai() { return temPai_; };
		Node* getPai() { return paiIndex; }
		void setPai(Node* v) {
			paiIndex = v;
			temPai_ = true;
		};

		Sala* getElemento() { return elemento; };

		void addAnexa(int novo) { anexasIndex.push_back(novo); };
		vector<int> getAnexas() { return anexasIndex; };
	};

	// Atributos
	vector<Node> salas;
	vector<Objeto> objetos;
	IObserver* observer;

	bool optimalPathHelper(queue<Node*> *salasExaminadas, bool isDireita, vector<Node*> *ponte);

public:
	Mapa() {};
	Mapa(vector<Sala*> salasRecebidas, IObserver *observer);

	void carregarSala(Sala *sala);

	queue<Sala*> optimalPath(Sala *_salaOrigem, Sala *_salaDestino);
	bool existeSala(string name);
	Sala* getSala(int index);
	Sala* getSala(string name);
};