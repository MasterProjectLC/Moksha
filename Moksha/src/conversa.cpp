#include "conversa.h"

Conversa::Conversa(set<string> participantes, string conversa, string sala) {
	this->participantes = participantes;
	this->conversa.load_file( ("files/conversas/" + conversa + ".xml").c_str() );
	it = this->conversa.child("Conversa").begin();
	this->sala = sala;
}

bool Conversa::participa(string nome) {
	return participantes.count(nome) > 0;
}

xml_node Conversa::proximaFala() {
	return *(it++);
}

bool Conversa::noFim() {
	return it == conversa.child("Conversa").end();
}


set<string> Conversa::getParticipantes(string removido) {
	set<string> retorno = getParticipantes();
	
	for (set<string>::iterator it = retorno.begin(); it != retorno.end(); it++) {
		if ((*it) == removido) {
			retorno.erase(it);
			break;
		}
	}

	return retorno;
}