#include "Amelie.h"

Amelie::Amelie(Map* m) : NPC{ m, "Amelie", 
("Amelie is a mysterious guest aboard the airship. An old, frail woman in her 70s, from both her apparent ethnicity and traditional clothing, "
"it’s safe to say that she’s some sort of indian religious figure - probably a monk. Her expression is distant, but her demeanor is gentle. "
"Why is she here and what does she want?")
, F, 0, 0 } {
	trackablePeople.insert("Santos");
}

void Amelie::setupActionsParticular() {
	
}


void Amelie::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "with_Santos", true);
}


void Amelie::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "with_Santos", true);
}


int Amelie::decideActionParticular(string action) {
	return descansar;
}


void Amelie::advancePlansExtra(string currentProcess) {

}


void Amelie::updateWorldExtra() {
	// describe current world state.
}