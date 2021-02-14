#pragma once
#include <vector>
#include <set>
#include <string>
#include "../libs/pugixml/src/pugixml.hpp"

using namespace std;
using namespace pugi;

class Conversa {
private:
	int convoStage;
	xml_document conversa;
	xml_node_iterator it;
	set<string> participantes;
	string sala;

public:
	Conversa() {}
	Conversa(string conversa, string sala);
	Conversa(string conversa, string sala, int stage);

	bool participa(string nome); // Se personagem citado participa da conversa ou não

	xml_node proximaFala();
	bool noFim();

	set<string> getParticipantes() { return set<string>(participantes.begin(), participantes.end()); }
	set<string> getParticipantes(string removido);

	string getSala() { return sala; }
	int getStage() { return convoStage; }
};
