#include "Santos.h"

Santos::Santos(Map* m) : NPC{ m, "Santos", 
("Albert Santos Wright is the loud, bold and extroverted half of the Blakewell and Wright Aviation Company. "
"A skilled pilot and a fierce fighter, he's known for personally testing every model himself prior to launch.\n"
"Santos is a strong englishman with a bulky body and an english moustache. Today, he's wearing a big, heavy fur coat and a pilot 'hat and goggles' combo.")
, F, 13, 12 } {

}

void Santos::setupActionsParticular() {
	addTrackablePeople("Hilda");
	addTrackablePeople("Baxter");
	addTrackablePeople("Tom");
	addTrackableConvo("santos_gun");
	goap_set_pre(&ap, "convo_santos_gun", "with_Baxter", true);

	addTrackableRoom("Mezzanine");
	goap_set_pre(&ap, "enter_CrewArea", "in_Mezzanine", true);
	goap_set_pst(&ap, "enter_CrewArea", "in_CrewArea", true);
	goap_set_pre(&ap, "move_Navigation", "in_CrewArea", true);
	addTrackableRoom("Navigation");
	goap_set_pre(&ap, "pilot", "in_Navigation", true);
	goap_set_pst(&ap, "pilot", "piloting", true);
}


void Santos::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "with_Hilda", true);
	goap_worldstate_set(&ap, &world, "with_Baxter", true);
	goap_worldstate_set(&ap, &world, "piloting", false);
}


void Santos::setupObjectivesParticular() {
	//goap_worldstate_set(&ap, &currentGoal.goal, "piloting", true);
	goap_worldstate_set(&ap, &currentGoal.goal, "convo_santos_gun", true);
}


int Santos::decideActionParticular(string action) {
	if (action == "pilot") {
		actionArgs.push_back("piloting.");
		return acaoNula;
	}

	if (action == "enter_CrewArea") {
		actionArgs.push_back("open");
		actionArgs.push_back("Crew Door");
		return interagir;
	}

	return descansar;

}


void Santos::updateProcessExtra(string currentProcess) {

}


void Santos::updateWorldExtra() {

}