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

	goap_set_pre(&ap, "take_photos", "in_ViewingLobby", true);
	goap_set_pst(&ap, "take_photos", "taking_photos", true);
	photos_taken = 0;

	goap_set_pre(&ap, "hear_presentation", "in_ViewingLobby", true);
	goap_set_pst(&ap, "hear_presentation", "the_medusa", true);
}


void Renard::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "in_RenardRoom", true);
	goap_worldstate_set(&ap, &world, "writing", false);
	goap_worldstate_set(&ap, &world, "taking_photos", false);
	goap_worldstate_set(&ap, &world, "the_medusa", false);
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
	if (action == "write") {
		actionArgs.push_back("writing.");
		return acaoNula;
	}

	if (action == "take_photos") {
		photos_taken++;
		actionArgs.push_back("taking photos.");
		return acaoNula;
	}

	if (action == "hear_presentation") {
		actionArgs.push_back("waiting.");
		return acaoNula;
	}

	return descansar;
}