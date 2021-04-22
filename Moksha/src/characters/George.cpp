#include "George.h"

George::George(Map* m) : NPC(m, "George") {

}

void George::setupActionsParticular() {
	addTrackablePeople("Jenna");
	addTrackablePeople("Willow");
	addTrackableRoom("Runway");
	addTrackableRoom("ViewingLobby");
	addTrackableRoom("DiningHall");

	addTrackableNull("wait_Runway", "waiting_Runway", "waiting", "Runway");
	addTrackableNull("have_breakfast", "breakfast", "having breakfast", "DiningHall");
	addTrackableNull("hear_presentation", "medusa", "waiting", "ViewingLobby");

	addTrackableRoom("GameRoom");
	addTrackableConvo("begin_card_game", "GameRoom");

	goap_set_pre(&ap, "wait_Jenna", "expecting_Jenna", true);
	goap_set_pst(&ap, "wait_Jenna", "with_Jenna", true);
	goap_set_cost(&ap, "wait_Jenna", 1);

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
	// describe current world state.
	if (lastSeen.hasKey("Jenna"))
		goap_set_cost(&ap, "search_Jenna", pathSize(currentRoom, mapp->getRoom(lastSeen.getValues("Jenna"))));
}


void George::setupProcessParticular(string currentProcess) {

}


int George::decideActionParticular(string action) {
	if (action == "wait_Jenna" || action == "hear_presentation") {
		actionArgs.push_back("waiting.");
		return acaoNula;
	}



	return descansar;
}