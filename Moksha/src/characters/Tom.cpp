#include "Tom.h"

Tom::Tom(Map* m) : NPC{ m, "Tom", 
"Thomas Wright is Santos' 17-year-old son, currently working under Hilda as a lowly crewman aboard the Medusa. "
"He seems to have quite a grudge against his father, wanting both to impress and ruin him.\n"
"Despite his father's strength, Tom relies more on technique and agility to perform tasks. He's currently wearing a white shirt, suspenders and a workers cap.", 
M, 10, 11 } {
	trackablePeople.insert("Santos");
}

void Tom::setupActionsParticular() {
	
}


void Tom::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "with_Santos", true);
}


void Tom::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "with_Santos", true);
}


int Tom::decideActionParticular(string action) {
	return descansar;
}


void Tom::advancePlansExtra(string currentProcess) {

}


void Tom::updateWorldExtra() {
	// describe current world state.
}