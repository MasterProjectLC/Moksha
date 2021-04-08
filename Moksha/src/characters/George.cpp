#include "George.h"

George::George(Map* m) : NPC{ m, "George", 
("George Maybourne, Jenna's uncle, is an United States Aeronautical Division General and the one representing the organization during today's voyage. "
"He seems kind and respectful, though Jenna has already told me of his... drinking problems.\n"
"He's an old man with a grizzled beard and a large body, wearing a fully condecorated blue aeronautical uniform and a military hat."),
M, 8, 13 } {

}

void George::setupActionsParticular() {
	addTrackablePeople("Jenna");
	addTrackablePeople("Willow");
	addTrackableRoom("Runway");
	addTrackableRoom("ViewingDeck");
	goap_set_pre(&ap, "wait_Runway", "in_Runway", true);
	goap_set_pst(&ap, "wait_Runway", "waiting_Runway", true);
}


void George::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "in_ViewingDeck", false);
	goap_worldstate_set(&ap, &world, "in_Runway", false);
	goap_worldstate_set(&ap, &world, "waiting_Runway", false);
	updateWorldExtra();
}


void George::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "in_ViewingDeck", true);
}


void George::updateWorldExtra() {
	// describe current world state.
	if (lastSeen.hasKey("Jenna"))
		goap_set_cost(&ap, "search_Jenna", pathSize(currentRoom, mapp->getRoom(lastSeen.getValues("Jenna"))));
}


void George::setupProcessParticular(string currentProcess) {

}


int George::decideActionParticular(string action) {
	if (action == "wait_Runway") {
		actionArgs.push_back("waiting.");
		return acaoNula;
	}
	return descansar;
}