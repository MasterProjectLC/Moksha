#include "sala.h"

Sala::Sala(string name, string textoInicial, vector<string> salasAnexasNomes) {
	this->name = name;
	this->textoInicial = textoInicial;
	this->salasAnexasNomes = salasAnexasNomes;
}

bool Sala::isSalaAnexa(string salaAnexaNome) {
	return (getSalaAnexaIndex(salaAnexaNome) != -1);
}

// GETTERS
int Sala::getSalaAnexaIndex(string alvo) {
	for (int i = 0; i < salasAnexasNomes.size(); i++) {
		if (salasAnexasNomes[i] == alvo) {
			return salasAnexasIndexes[i];
		}
	}

	return -1;
}

int Sala::getSalaAnexaIndex(int i) {
	return salasAnexasIndexes[i];
};


string Sala::getSalaAnexaNome(int i) {
	return salasAnexasNomes[i];
};