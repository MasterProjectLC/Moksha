#include "George.h"

George::George(Map* m) : NPC(m, "George") {

}

void George::setupActionsParticular() {
	addTrackablePeople("Jenna", vector<string>({ "JennaRoom" }));
	addTrackablePeople("Willow");
	addTrackableRoom("Runway");
	addTrackableRoom("ViewingLobby");
	addTrackableRoom("JennaRoom");
	addTrackableRoom("GeorgeRoom");
	addTrackableRoom("DiningHall");

	addTrackableNull("wait_Runway", "waiting_Runway", "waiting", "Runway");
	addTrackableNull("have_breakfast", "breakfast", "having breakfast", "DiningHall");
	addTrackableNull("write", "writing", "GeorgeRoom");
	addTrackableNull("hear_presentation", "medusa", "waiting", "ViewingLobby");

	addTrackableRoom("GameRoom");
	addTrackableConvo("begin_card_game", "in_GameRoom");

	goap_set_pre(&ap, "search_Jenna", "expecting_Jenna", false);
	goap_set_pre(&ap, "wait_Jenna", "expecting_Jenna", true);
	goap_set_pst(&ap, "wait_Jenna", "with_Jenna", true);

	addTrackableNull("play", "playing", "GameRoom");
	goap_set_pre(&ap, "play", "with_Jenna", true);
}


void George::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "expecting_Jenna", true);
}


void George::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "breakfast", true);
}


void George::updateWorldExtra() {
	
}


void George::setupProcessParticular(string currentProcess) {

}


int George::decideActionParticular(string action) {
	if (action == "wait_Jenna") {
		actionArgs.push_back("waiting.");
		return acaoNula;
	}

	return descansar;
}