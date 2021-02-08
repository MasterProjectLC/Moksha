#pragma once
#include <vector>
#include <string>
#include "objeto.h"

using namespace std;

class Sala {
	// Atributos
private:
	string nome;
	string codinome;
	string textoInicial;
	vector<string> salasAnexasNomes;
	int index;

	vector<string> objetoNomes;
	vector<Objeto> objetos;

public:
	Sala() {};

	// Setup
	Sala(int index, string nome, string textoInicial, vector<string> salasAnexasNomes, vector<string> objetoNomes);
	Sala(int index, string nome, string codinome, string textoInicial, vector<string> salasAnexasNomes, vector<string> objetoNomes);
	void setObjetoNomes(vector<string> objetoNomes) { this->objetoNomes = objetoNomes; }

	void setupObjetos(vector<Objeto> objetos) { this->objetos = objetos; }
	void addObjeto(Objeto objeto) { objetos.push_back(objeto); }
	void removeObjeto(Objeto objeto);
	void limparObjetos() { objetos.clear(); objetoNomes.clear(); }

	// Getters
	string getNome() { return nome; }
	string getCodinome() { return codinome; }
	string getTextoInicial() { return textoInicial; }
	int getIndex() { return index; }

	bool isSalaAnexa(string salaAnexaNome);
	string getSalaAnexaNome(int i);
	int getSalaAnexaCount() { return salasAnexasNomes.size(); }

	bool possuiObjeto(string nome);
	Objeto* getObjeto(string nome);
	Objeto* getObjeto(int index) { return &objetos[index]; }
	vector<Objeto> getObjetos() { return objetos; }
	vector<string> getObjetoNomes();

};