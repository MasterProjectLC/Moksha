#include "conversa.h"

Conversa::Conversa(string conversa, string sala) {
	convoStage = 0;
	this->conversa.load_file( ("files/conversas/" + conversa + ".xml").c_str() );
	it = this->conversa.child("Conversa").begin();
	this->sala = sala;

	// Inserir participantes
	for (xml_node_iterator ait = this->conversa.child("Participants").begin(); ait != this->conversa.child("Participants").end(); ait++) {
		this->participantes.insert(ait->name());
	}
}

Conversa::Conversa(string conversa, string sala, int stage) : Conversa(conversa, sala) {
	for (int i = 0; i < stage; i++) {
		proximaFala();
	}
}

bool Conversa::participa(string nome) {
	return participantes.count(nome) > 0;
}

xml_node Conversa::proximaFala() {
	convoStage++;
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