#include "Amelie.h"

Amelie::Amelie(Map* m) : NPC{ m, "Amelie", 
("Amelie is a mysterious guest aboard the airship. An old, frail woman in her 70s, from both her apparent ethnicity and traditional clothing, "
"it's safe to say that she’s some sort of indian religious figure - probably a monk. Her expression is distant, but her demeanor is gentle. "
"Why is she here and what does she want?")
, F, 0, 0 } {
	trackablePeople.insert("Santos");
}

void Amelie::setupActionsParticular() {
	addTrackableRoom("AmelieRoom");
	goap_set_pre(&ap, "meditate", "in_AmelieRoom", true);
	goap_set_pst(&ap, "meditate", "meditating", true);
}


void Amelie::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "meditating", false);
}


void Amelie::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "meditating", true);
}

void Amelie::updateWorldExtra() {
	
}

int Amelie::decideActionParticular(string action) {
	if (action == "meditate") {
		actionArgs.push_back("meditating.");
		return acaoNula;
	}

	return descansar;
}


void Amelie::updateProcessExtra(string currentProcess) {

}