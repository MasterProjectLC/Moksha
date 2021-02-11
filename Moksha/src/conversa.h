#pragma once
#include <vector>
#include <set>
#include <string>
#include "../libs/pugixml/src/pugixml.hpp"

using namespace std;
using namespace pugi;

class Conversa {
private:
	xml_document conversa;
	xml_node_iterator it;
	set<string> participantes;
	string sala;

public:
	Conversa() {}
	Conversa(set<string> participantes, string conversa, string sala);

	bool participa(string nome); // Se personagem citado participa da conversa ou não

	xml_node proximaFala();
	bool noFim();

	set<string> getParticipantes() { return set<string>(participantes.begin(), participantes.end()); }
	set<string> getParticipantes(string removido);

	string getSala() { return sala; }
};
