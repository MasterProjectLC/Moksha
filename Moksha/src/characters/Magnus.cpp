#include "Magnus.h"

Magnus::Magnus(Map* m) : NPC{ m, "Magnus", 
("Magnus Flettner is the german inventor responsible for designing the airship Medusa. "
"Today, he was brought along to explain details of the ship to any interested guests and to perform specialized maintenance if needed.\n"
"He's slim, somewhat old and wears delicate round glasses. Right now, he's outfitted with a golden bow tie, a red vest and a dark suit."), 
M, 4, 3 } {

}

void Magnus::setupActionsParticular() {
	addTrackablePeople("Amelie");
	addTrackablePeople("Baxter");
	addTrackablePeople("Santos");
	addTrackableRoom("Study");
	goap_set_pre(&ap, "study", "in_Study", true);
	goap_set_pst(&ap, "study", "studying", true);
}


void Magnus::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "studying", false);
}


void Magnus::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "studying", true);
}


void Magnus::updateWorldExtra() {
	// describe current world state.
}

void Magnus::updateProcessExtra(string currentProcess) {

}

int Magnus::decideActionParticular(string action) {
	if (action == "study") {
		actionArgs.push_back("studying.");
		return acaoNula;
	}
	return descansar;
}