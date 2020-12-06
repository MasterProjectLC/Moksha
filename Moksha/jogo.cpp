#include "jogo.h"

vector<Conceito> Jogo::getInventario() {
	return inventario.getInventario();
}

void Jogo::receberArgs(vector<string> args) {
	switch (args.size()) {
	case 2:
		if (args.at(0) == "obter") {
			inventario.addConceito(args.at(1));
			notify();
		}
		break;
	}
}
