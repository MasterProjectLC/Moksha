#include "Renard.h"

Renard::Renard(Map* m) : NPC{ m, "Renard", 
("Claude Renard is a french scandal reporter and Jenna's rival. Paul is his assistant. "
"While he doesn’t seem to care much for the sciences, even Jenna admits he has a good eye for art.\n"
"Renard has a prominent french moustache and a stylish hairstyle .Currently, he wears a white bow tie, a dark green frock coat and a top hat."), 
M, 3, 5 } {

}

void Renard::setupActionsParticular() {
	addTrackablePeople("Willow");
	addTrackablePeople("Liz");
	addTrackablePeople("Paul");
	addTrackableRoom("RenardRoom");
	goap_set_pre(&ap, "write", "in_RenardRoom", true);
	goap_set_pst(&ap, "write", "writing", true);
}


void Renard::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "in_RenardRoom", true);
	goap_worldstate_set(&ap, &world, "writing", false);
}


void Renard::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "writing", true);
}


void Renard::updateProcessExtra(string currentProcess) {

}


void Renard::updateWorldExtra() {
	// describe current world state.
}


int Renard::decideActionParticular(string action) {
	if (action == "write") {
		actionArgs.push_back("writing.");
		return acaoNula;
	}

	return descansar;
}