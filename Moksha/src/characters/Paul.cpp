#include "Paul.h"

Paul::Paul(Map* m) : NPC{ m, "Paul", 
"Paul Davis is Renard's quiet assistant. While seemingly curious about the inner workings of the ship, he doesn't seem that interested in helping Renard.\n"
"Paul has a rough ginger beard and some untamed hair.Right now, he wears a closed leather jacket and a discreet newsy cap.", 
M, 7, 9 } {
	trackablePeople.insert("Santos");
}

void Paul::setupActionsParticular() {
	
}


void Paul::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "with_Santos", true);
}


void Paul::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "with_Santos", true);
}


int Paul::decideActionParticular(string action) {
	return descansar;
}


void Paul::advancePlansExtra(string currentProcess) {

}


void Paul::updateWorldExtra() {
	// describe current world state.
}