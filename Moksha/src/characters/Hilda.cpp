#include "Hilda.h"

Hilda::Hilda(Map* m) : NPC{ m, "Hilda", 
("Hilda Bernhard is the strong, no-nonsense captain commanding the crew of Medusa. When she’s not shouting out orders to the workers at the engine rooms, " 
"Hilda pilots the ship along with Santos.\n"
"She’s a big, towering woman, currently wearing a large blue jacket, heavy leather boots and a black tie."), 
F, 11, 6 } {
	trackablePeople.insert("Santos");
}

void Hilda::setupActionsParticular() {

}


void Hilda::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "with_Santos", true);
}


void Hilda::setupObjectivesParticular() {

}


int Hilda::decideActionParticular(string acao) {
	return descansar;
}


void Hilda::advancePlansExtra(string currentProcess) {

}


void Hilda::updateWorldExtra() {
	// describe current world state.
}