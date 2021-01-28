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

	vector<string> objetoNomes;
	vector<Objeto> objetos;

public:
	Sala() {};

	// Setup
	Sala(string name, string textoInicial, vector<string> salasAnexasNomes, vector<string> objetoNomes);
	void setupObjetos(vector<Objeto> objetos) { this->objetos = objetos; }
	void addObjeto(Objeto objeto) { objetos.push_back(objeto); }
	void removeObjeto(Objeto objeto);
	void limparObjetos() { objetos.clear(); }

	// Getters
	string getName() { return name; }
	string getTextoInicial() { return textoInicial; }

	bool isSalaAnexa(string salaAnexaNome);
	string getSalaAnexaNome(int i);
	int getSalaAnexaCount() { return salasAnexasNomes.size(); }

	bool possuiObjeto(string nome);
	Objeto* getObjeto(string nome);
	Objeto* getObjeto(int index) { return &objetos[index]; }
	vector<Objeto> getObjetos() { return objetos; }
	vector<string> getObjetoNomes();

};