#pragma once
#include <vector>
#include <string>
#include "object.h"

using namespace std;

class Sala {
	// Atributos
private:
	string name;
	string codename;
	string initialText;
	vector<string> salasAnexasNomes;
	int index;

	vector<string> objectNames;
	vector<Object> objects;

public:
	Sala() {};

	// Setup
	Sala(int index, string name, string textoInicial, vector<string> salasAnexasNomes, vector<string> objetoNomes);
	Sala(int index, string name, string codinome, string textoInicial, vector<string> salasAnexasNomes, vector<string> objetoNomes);
	void setObjectNames(vector<string> objetoNomes) { this->objectNames = objetoNomes; }

	void setupObjects(vector<Object> objetos) { this->objects = objetos; }
	void addObject(Object objeto) { objects.push_back(objeto); }
	void removeObject(Object objeto);
	void limparObjects() { objects.clear(); objectNames.clear(); }

	// Getters
	string getName() { return name; }
	string getCodename() { return codename; }
	string getInitialText() { return initialText; }
	int getIndex() { return index; }

	bool isSalaAnexa(string salaAnexaNome);
	string getAdjacentRoomName(int i);
	int getSalaAnexaCount() { return salasAnexasNomes.size(); }

	bool hasObject(string name);
	Object* getObject(string name);
	Object* getObject(int index) { return &objects[index]; }
	vector<Object> getObjects() { return objects; }
	vector<string> getObjectNames();

};