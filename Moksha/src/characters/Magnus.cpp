#include "Magnus.h"

Magnus::Magnus(Map* m) : NPC{ m, "Magnus", 
("Magnus Flettner is the german inventor responsible for designing the airship Medusa. "
"Today, he was brought along to explain details of the ship to any interested guests and to perform specialized maintenance if needed.\n"
"He's slim, somewhat old and wears delicate round glasses. Right now, he's outfitted with a golden bow tie, a red vest and a dark suit."), 
M, 4, 3 } {
	trackablePeople.insert("Santos");
}

void Magnus::setupActionsParticular() {
	
}


void Magnus::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "with_Santos", true);
}


void Magnus::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "with_Santos", true);
}


int Magnus::decideActionParticular(string action) {
	return descansar;
}


void Magnus::advancePlansExtra(string currentProcess) {

}


void Magnus::updateWorldExtra() {
	// describe current world state.
}