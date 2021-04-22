#include "Liz.h"

Liz::Liz(Map* m) : NPC(m, "Liz") {

}

void Liz::setupActionsParticular() {
	addTrackablePeople("Santos");
	addTrackableRoom("LizRoom");
	addTrackableNull("paint", "painting", "LizRoom");
}


void Liz::setupWorldParticular() {

}


void Liz::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "painting", true);
}


void Liz::updateWorldExtra() {

}


void Liz::setupProcessParticular(string currentProcess) {

}


int Liz::decideActionParticular(string action) {
	return descansar;
}