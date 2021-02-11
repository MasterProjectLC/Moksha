#pragma once
#include "sala.h"
#include "inventario.h"
#include "dictionary.h"
#include "IObservable.h"

using namespace std;

class Personagem: public IObservable {
protected:
	string nome;
	string notifyText;
	set<string> notifyTargets;
	Dictionary<string> ultimoAvistamento;
	set<Sala*> salasChecadas;

	int forca;
	int destreza;
	bool inconsciente;
	bool morto;
	int genero;
	
	Sala *salaAtual;
	Inventario inventario;

public:
	Personagem(int genero, int forca, int destreza) {
		this->genero = genero;
		this->forca = forca;
		this->destreza = destreza;
		inconsciente = false;
		morto = false;
	}

	string getNome() { return nome; }

	vector<Item> getInventario() { return inventario.getItens(); }
	void addConceito(string nome) { inventario.addConceito(nome); }
	void addItem(string nome, set<string> acoes) { inventario.addItem(nome, acoes); }

	void setSalaAtual(Sala *sala) {	salaAtual = sala; }
	Sala* getSalaAtual() { return salaAtual; }
	int getGenero() { return genero; }
	int getForca() { return forca; }
	int getDestreza() { return destreza; }
	bool isInconsciente() { return inconsciente || morto; }
	bool isMorto() { return morto; }

	virtual void tomarAcao() {}

	enum { M, F, imprimir, mover, mencionar, falar, atacar, descansar, conversar };
	void printText(string str);
	void move(string str);
	void move(Sala sala);
	void mention(string obj, set<string> receivers);
	void attack(string target);
	void say(string topico, string str, set<string> receivers);
	void rest();
	void talk(string convo, set<string> participants);

	string getNotifyText() { return notifyText; }
	set<string> getNotifyTargets() { return notifyTargets; }

	virtual bool temCondicao(string info) { return false; }

	virtual void executarReacao(string topico, string frase, string remetente) {};
	virtual void verSala(vector<Personagem*> pessoasNaSala) {};
	virtual void verPessoaMovendo(Personagem* pessoa, string outraSala, bool entrando) {};
	virtual void serAtacado(Personagem* atacante) {
		if (atacante->getForca() >= forca)
			inconsciente = true;
	};
};
