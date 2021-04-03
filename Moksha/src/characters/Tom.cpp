#include "Tom.h"

Tom::Tom(Map* m) : NPC{ m, "Tom", 
"Thomas Wright is Santos' 17-year-old son, currently working under Hilda as a lowly crewman aboard the Medusa. "
"He seems to have quite a grudge against his father, wanting both to impress and ruin him.\n"
"Despite his father's strength, Tom relies more on technique and agility to perform tasks. He's currently wearing a white shirt, suspenders and a workers cap.", 
M, 10, 11 } {
	trackablePeople.insert("Santos");
}

void Tom::setupActionsParticular() {
	addTrackableRoom("ControlRoom");
	addTrackableRoom("NorthEngines");
	goap_set_pre(&ap, "work", "in_NorthEngines", true);
	goap_set_pst(&ap, "work", "working", true);
}


void Tom::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "in_ControlRoom", false);
	goap_worldstate_set(&ap, &world, "in_NorthEngines", true);
	goap_worldstate_set(&ap, &world, "working", false);
}


void Tom::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "working", true);
}

void Tom::updateWorldExtra() {
	// describe current world state.
}

void Tom::updateProcessExtra(string currentProcess) {

}

int Tom::decideActionParticular(string action) {
	if (action == "work") {
		actionArgs.push_back("working.");
		return acaoNula;
	}
	
	return descansar;
}