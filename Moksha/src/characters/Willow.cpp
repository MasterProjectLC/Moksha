#include "Willow.h"

Willow::Willow(Map* m) : NPC{ m, "Willow", 
("Willow Blakewell, formerly known as Willow Niedenberg, is Baxter's wife, Liz' sister and a member of the austrian nobility. "
"As far as I know, she seems to be a compassionate and kind individual.\n"
"Her hair is long and elaborate, tranced in an intricate fashion. Today, she's wearing a golden and blue dress.")
, F, 1, 1 } {
	trackablePeople.insert("Baxter");
}

void Willow::setupActionsParticular() {
	
}


void Willow::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "with_Baxter", true);
}


void Willow::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "with_Baxter", true);
}


int Willow::decideActionParticular(string action) {
	return descansar;
}


void Willow::advancePlansExtra(string currentProcess) {

}


void Willow::updateWorldExtra() {
	// describe current world state.
}