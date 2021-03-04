#include "sala.h"

Sala::Sala(int index, string nome, string textoInicial, vector<string> salasAnexasNomes, vector<string> objectNames) {
	this->index = index;
	this->nome = nome;
	this->codinome = nome;
	this->textoInicial = textoInicial;
	this->salasAnexasNomes = salasAnexasNomes;
	this->objectNames = objectNames;
}

Sala::Sala(int index, string nome, string codinome, string textoInicial, vector<string> salasAnexasNomes, vector<string> objectNames) {
	this->index = index;
	this->nome = nome;
	this->codinome = codinome;
	this->textoInicial = textoInicial;
	this->salasAnexasNomes = salasAnexasNomes;
	this->objectNames = objectNames;
}


void Sala::removeObject(Object objeto) {
	bool found = false;
	for (int i = 0; i < objects.size(); i++) {
		if (found) {
			objects[i].setNotifyID(objects[i].getNotifyID()-1);
			objects[i-1] = objects[i];
			objectNames[i - 1] = objectNames[i];

		} else if (objects[i].getName().compare(objeto.getName()) == 0) {
			found = true;
		}
	}

	if (found) {
		objects.resize(objects.size() - 1);
		objectNames.resize(objectNames.size() - 1);
	}
}

bool Sala::isSalaAnexa(string salaAnexaNome) {
	for (int i = 0; i < salasAnexasNomes.size(); i++)
		if (salaAnexaNome == salasAnexasNomes[i])
			return true;
	return false;
}

// GETTERS
string Sala::getSalaAnexaNome(int i) {
	return salasAnexasNomes[i];
};


bool Sala::hasObject(string nome) {
	for (int i = 0; i < objects.size(); i++) {
		if (nome.compare(objects[i].getName()) == 0)
			return true;
	}
	return false;
}


#include <stdexcept>
Object* Sala::getObject(string nome) {
	for (int i = 0; i < objects.size(); i++) {
		if (nome.compare(objects[i].getName()) == 0)
			return &objects[i];
	}

	throw invalid_argument("There's no object with that name :(");
}


vector<string> Sala::getObjectNames() {
	return objectNames;
};
