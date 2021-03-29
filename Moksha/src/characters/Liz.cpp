#include "Liz.h"

Liz::Liz(Map* m) : NPC{ m, "Liz", 
("Liz Niedenberg is Willow’s sister and one of the last members of the austrian Niedenberg noble family. "
"She's a cunning and perceptive woman, and seems willing to do anything necessary to save her family's honor.\n"
"Her hair is long and straight, and her face is a bit more angular than Willow's. Today, she's wearing an elaborate red and white dress."),
F, 2, 2 } {
	trackablePeople.insert("Santos");
}

void Liz::setupActionsParticular() {
	
}


void Liz::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "with_Santos", true);
}


void Liz::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "with_Santos", true);
}


int Liz::decideActionParticular(string action) {
	return descansar;
}


void Liz::advancePlansExtra(string currentProcess) {

}


void Liz::updateWorldExtra() {
	// describe current world state.
}