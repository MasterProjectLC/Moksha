#include "George.h"

George::George(Map* m) : NPC{ m, "George", 
("George Maybourne, Jenna’s uncle, is an United States Aeronautical Division General and the one representing the organization during today’s voyage. "
"He seems kind and respectful, though Jenna has already told me of his… drinking problems.\n"
"He’s an old man with a grizzled beard and a large body, wearing a fully condecorated blue aeronautical uniform and a military hat."),
M, 8, 13 } {
	trackablePeople.insert("Jenna");
}

void George::setupActionsParticular() {
	goap_set_pre(&ap, "search_Jenna", "Jenna_dead", false);
	goap_set_pst(&ap, "search_Jenna", "with_Jenna", true);
}


void George::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "with_Jenna", false);
	goap_worldstate_set(&ap, &world, "Jenna_dead", false);
}


void George::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "with_Jenna", true);
}


void George::updateWorldExtra() {
	// describe current world state.
	if (lastSeen.hasKey("Jenna"))
		goap_set_cost(&ap, "search_Jenna", pathSize(currentRoom, mapp->getRoom(lastSeen.getValues("Jenna"))));
}


int George::decideActionParticular(string action) {
	if (action.substr(0, 7).compare("search_") == 0) {
		actionArgs.push_back(nextRoomInPath());
		return mover;
	}

	return descansar;
}


void George::advancePlansExtra(string currentProcess) {

}