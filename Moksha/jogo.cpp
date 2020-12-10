#include "jogo.h"

string Jogo::concatenarArgs(vector<string> args, int inicio) {
	string r = args.at(inicio);
	for (int i = inicio+1; i < args.size(); i++) {
		r += " " + args.at(i);
	}
	return r;
}

vector<Conceito> Jogo::getInventario() {
	return inventario.getInventario();
}

void Jogo::receberArgs(vector<string> args) {
	if (args.size() >= 2 && args.at(0) == "obter") {
		inventario.addConceito(concatenarArgs(args, 1));
		notify();
	}
}