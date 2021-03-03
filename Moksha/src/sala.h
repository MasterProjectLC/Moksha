#pragma once
#include <vector>
#include <string>
#include "object.h"

using namespace std;

class Sala {
	// Atributos
private:
	string nome;
	string codinome;
	string textoInicial;
	vector<string> salasAnexasNomes;
	int index;

	vector<string> objectNames;
	vector<Object> objects;

public:
	Sala() {};

	// Setup
	Sala(int index, string nome, string textoInicial, vector<string> salasAnexasNomes, vector<string> objetoNomes);
	Sala(int index, string nome, string codinome, string textoInicial, vector<string> salasAnexasNomes, vector<string> objetoNomes);
	void setObjectNames(vector<string> objetoNomes) { this->objectNames = objetoNomes; }

	void setupObjects(vector<Object> objetos) { this->objects = objetos; }
	void addObject(Object objeto) { objects.push_back(objeto); }
	void removeObject(Object objeto);
	void limparObjects() { objects.clear(); objectNames.clear(); }

	// Getters
	string getNome() { return nome; }
	string getCodinome() { return codinome; }
	string getTextoInicial() { return textoInicial; }
	int getIndex() { return index; }

	bool isSalaAnexa(string salaAnexaNome);
	string getSalaAnexaNome(int i);
	int getSalaAnexaCount() { return salasAnexasNomes.size(); }

	bool hasObject(string nome);
	Object* getObject(string nome);
	Object* getObject(int index) { return &objects[index]; }
	vector<Object> getObjects() { return objects; }
	vector<string> getObjectNames();

};