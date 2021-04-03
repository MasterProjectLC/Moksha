#include "Hilda.h"

Hilda::Hilda(Map* m) : NPC{ m, "Hilda", 
("Hilda Bernhard is the strong, no-nonsense captain commanding the crew of Medusa. When she's not shouting out orders to the workers at the engine rooms, " 
"Hilda pilots the ship along with Santos.\n"
"She's a big, towering woman, currently wearing a large blue jacket, heavy leather boots and a black tie."), 
F, 11, 6 } {
	trackablePeople.insert("Santos");
}

void Hilda::setupActionsParticular() {
	addTrackableRoom("Mezzanine");
	goap_set_pre(&ap, "enter_CrewArea", "in_Mezzanine", true);
	goap_set_pst(&ap, "enter_CrewArea", "in_CrewArea", true);
	goap_set_pre(&ap, "move_Navigation", "in_CrewArea", true);
	addTrackableRoom("Navigation");
	goap_set_pre(&ap, "pilot", "in_Navigation", true);
	goap_set_pst(&ap, "pilot", "piloting", true);
}


void Hilda::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "with_Santos", true);
	goap_worldstate_set(&ap, &world, "piloting", false);
}


void Hilda::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "piloting", true);
}


int Hilda::decideActionParticular(string action) {
	if (action == "pilot") {
		actionArgs.push_back("piloting.");
		return acaoNula;
	}

	return descansar;
}


void Hilda::updateProcessExtra(string currentProcess) {

}


void Hilda::updateWorldExtra() {
	// describe current world state.
}