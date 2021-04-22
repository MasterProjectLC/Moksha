#include "Renard.h"

Renard::Renard(Map* m) : NPC(m, "Renard") {

}

void Renard::setupActionsParticular() {
	addTrackablePeople("Willow");
	addTrackablePeople("Liz");
	addTrackablePeople("Paul");
	addTrackableRoom("RenardRoom");
	addTrackableRoom("ViewingLobby");

	goap_set_pre(&ap, "write", "in_RenardRoom", true);
	goap_set_pst(&ap, "write", "writing", true);
	addTrackableNull("write", "writing", "RenardRoom");

	addTrackableNull("take_photos", "taking_photos", "taking photos", "ViewingLobby");
	photos_taken = 0;

	goap_set_pre(&ap, "hear_presentation", "in_ViewingLobby", true);
	goap_set_pst(&ap, "hear_presentation", "medusa", true);
	addTrackableNull("hear_presentation", "medusa", "waiting", "ViewingLobby");
}


void Renard::setupWorldParticular() {

}


void Renard::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "writing", true);
}


void Renard::setupProcessParticular(string currentProcess) {

}


void Renard::updateWorldExtra() {
	if (photos_taken >= 10)
		goap_worldstate_set(&ap, &world, "taking_photos", true);
}


int Renard::decideActionParticular(string action) {
	return descansar;
}