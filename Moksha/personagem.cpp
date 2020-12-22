#include "personagem.h"

bool Personagem::isAcaoValida(string acao) {
	for (int i = 0; i < acoesBasicas.size(); i++)
		if (acoesBasicas[i].compare(acao) == 0)
			return true;

	for (int i = 0; i < getInventario().size(); i++)
		if (getInventario()[i].isAcaoValida(acao))
			return true;

	return false;
}

