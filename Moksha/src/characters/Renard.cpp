#include "Renard.h"

Renard::Renard(Map* m) : NPC{ m, "Renard", 
("Claude Renard is a french scandal reporter and Jenna's rival. Paul is his assistant. "
"While he doesn’t seem to care much for the sciences, even Jenna admits he has a good eye for art.\n"
"Renard has a prominent french moustache and a stylish hairstyle .Currently, he wears a white bow tie, a dark green frock coat and a top hat."), 
M, 3, 5 } {
	trackablePeople.insert("Willow");
}

void Renard::setupActionsParticular() {
	
}


void Renard::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "with_Willow", true);
}


void Renard::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "with_Willow", true);
}


int Renard::decideActionParticular(string action) {
	return descansar;
}


void Renard::advancePlansExtra(string currentProcess) {

}


void Renard::updateWorldExtra() {
	// describe current world state.
}