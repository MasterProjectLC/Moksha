#include "Liz.h"

Liz::Liz(Map* m) : NPC(m, "Liz") {

}

void Liz::setupActionsParticular() {
	addTrackablePeople("Santos", vector<string>({"SantosRoom", "GameRoom"}));
	addTrackablePeople("George", vector<string>({ "GameRoom", "GeorgeRoom"}));

	addTrackableRoom("LizRoom");
	addTrackableRoom("SantosRoom");
	addTrackableRoom("GameRoom");

	addTrackableConvo("liz_card_game", "with_George");

	addTrackableNull("paint", "painting", "LizRoom");
	addTrackableNull("play", "playing", "GameRoom");


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