#pragma once
#include <vector>
#include <string>

using namespace std;

class Sala {
	// Atributos
private:
	string name;
	string textoInicial;
	vector<string> salasAnexasNomes;
	vector<int> salasAnexasIndexes;

public:
	Sala() {};
	Sala(string name, string textoInicial, vector<string> salasAnexasNomes);
	void setupAnexas(vector<int> salasAnexas) { this->salasAnexasIndexes = salasAnexas; };

	string getName() { return name; }
	string getTextoInicial() { return textoInicial; };

	bool isSalaAnexa(string salaAnexaNome);
	int getSalaAnexaIndex(string alvo);
	int getSalaAnexaIndex(int i);
	string getSalaAnexaNome(int i);

	int getSalaAnexaCount() { return salasAnexasNomes.size(); }

};