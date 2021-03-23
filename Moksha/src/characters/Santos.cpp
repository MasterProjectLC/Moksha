#include "Santos.h"

Santos::Santos(Map* m) : NPC{ m, "Santos", 
("Albert Santos Wright is the loud, bold and extroverted half of the Blakewell and Wright Aviation Company. "
"A skilled pilot and a fierce fighter, he's known for personally testing every model himself prior to launch.\n"
"Santos is a strong englishman with a bulky body and an english moustache. Today, he's wearing a big, heavy fur coat and a pilot 'hat and googles' combo.")
, F, 13, 12 } {
	trackablePeople.insert("Hilda");
	trackablePeople.insert("Baxter");
	trackablePeople.insert("Jenna");
}

void Santos::setupActionsParticular() {
	goap_set_pre(&ap, "talk_no_gun_Hilda", "with_Hilda", true);
	goap_set_pst(&ap, "talk_no_gun_Hilda", "talk_no_gun", true);

	goap_set_pre(&ap, "talk_no_gun_Baxter", "with_Baxter", true);
	goap_set_pst(&ap, "talk_no_gun_Baxter", "talk_no_gun", true);
}


void Santos::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "with_Hilda", true);
	goap_worldstate_set(&ap, &world, "with_Baxter", true);
	goap_worldstate_set(&ap, &world, "talk_no_gun", false);
}


void Santos::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "talk_no_gun", true);
}


int Santos::decideActionParticular(string action) {
	if (action == "talk_no_gun_Hilda" || action == "talk_no_gun_Baxter") {
		actionArgs.push_back("santos_gun");
		goap_worldstate_set(&ap, &world, "talk_no_gun", true);
		return conversar;
	}
	return descansar;

}


void Santos::advancePlansExtra(string currentProcess) {

}


void Santos::updateWorldExtra() {

}