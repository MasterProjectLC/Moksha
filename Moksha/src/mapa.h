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
	class Node { // Classe utilizada pelos métodos de procura
	private:
		Sala *elemento; // Sala representada pelo node
		int visitado; // Se node foi visitado pelo método de procura ou não
		vector<int> anexasIndex; // Index das anexas
		Node* paiIndex; // Index do node que originou este node
		bool temPai_; // Se já foi explorado

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
			if (v == this)
				temPai_ = false;
			else {
				paiIndex = v;
				temPai_ = true;
			}
		};

		Sala* getElemento() { return elemento; };

		void addAnexa(int novo) { anexasIndex.push_back(novo); };
		vector<int> getAnexas() { return anexasIndex; };

		bool isAnexa(int a) { 
			for (int i = 0; i < anexasIndex.size(); i++) { 
				if (anexasIndex[i] == a) {
					return true;
				}
			}
			return false;
		}
	};

	// Atributos
	int MAX_OBJECT_COUNT = 100;
	vector<Node> salas;
	vector<Object> objects;
	IObserver* observer;

	void breadthSearchHelper(queue<Sala*> *retorno, stack<int> &st, int salaChecada);

public:
	Mapa() {};
	Mapa(vector<Sala*> salasRecebidas, IObserver *observer);

	void carregarSala(Sala *sala);
	Object* getObject(int id);

	queue<Sala*> optimalPath(Sala *_salaOrigem, Sala *_salaDestino);
	queue<Sala*> breadthSearch(Sala *_salaOrigem);
	bool existeSala(string name);
	Sala* getSala(int index);
	Sala* getSala(string name);

	void limparVisitado();
};