#pragma once
#include <vector>
#include <string>
#include "objeto.h"

using namespace std;

class Sala {
	// Atributos
private:
	string name;
	string textoInicial;
	vector<string> salasAnexasNomes;
	vector<int> salasAnexasIndexes;

	vector<string> objetoNomes;
	vector<Objeto> objetos;

public:
	Sala() {};

	// Setup
	Sala(string name, string textoInicial, vector<string> salasAnexasNomes, vector<string> objetoNomes);
	void setupAnexas(vector<int> salasAnexas) { this->salasAnexasIndexes = salasAnexas; };
	void setupObjetos(vector<Objeto> objetos) { this->objetos = objetos; };
	void addObjeto(Objeto objeto) { objetos.push_back(objeto); };

	void sairSala();

	// Getters
	string getName() { return name; }
	string getTextoInicial() { return textoInicial; };

	bool isSalaAnexa(string salaAnexaNome);
	int getSalaAnexaIndex(string alvo);
	int getSalaAnexaIndex(int i);
	string getSalaAnexaNome(int i);
	int getSalaAnexaCount() { return salasAnexasNomes.size(); }

	vector<Objeto> getObjetos() { return objetos; };
	vector<string> getObjetoNomes() { return objetoNomes; };

};