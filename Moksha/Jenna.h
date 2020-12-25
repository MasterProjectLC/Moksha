#pragma once
#include "npc.h"

using namespace std;

class Jenna: public NPC {
private:

public:
	Jenna() {}
	Jenna(Mapa* m);
	void takeAction();
};