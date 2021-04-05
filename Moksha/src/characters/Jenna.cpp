#include "Jenna.h"

Jenna::Jenna(Map* m) : NPC{m, "Jenna", 
("Jenna Maybourne, George's niece, is an american 'innovation journalist', a title she coined herself."
"She's also my boss. Overall, I find her somewhat laidback and playful, simultaneously taking her job seriously and having fun along the way. "
"Her connection to her father, who was a prominent aeronaut, sparked her interest for technological marvels and allowed her to learn a lot about airships.\n"
"She's currently wearing a brown vest, a bowler hat and big, round googles. Her hair is shorter than most women."),
F, 9, 10} {

}

void Jenna::setupActionsParticular() {
	addTrackablePeople("Elliot");
	addTrackablePeople("Magnus");
	addTrackablePeople("Renard");
	addTrackablePeople("George");
	addTrackableRoom("Runway");
	addTrackableRoom("ViewingDeck");
	goap_set_pre(&ap, "wait_Runway", "in_Runway", true);
	goap_set_pst(&ap, "wait_Runway", "waiting_Runway", true);
}


void Jenna::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "in_ViewingDeck", false);
	goap_worldstate_set(&ap, &world, "in_Runway", false);
	goap_worldstate_set(&ap, &world, "waiting_Runway", false);
	updateWorldExtra();
}


void Jenna::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "in_ViewingDeck", true);
}


void Jenna::updateWorldExtra() {

}


void Jenna::updateProcessExtra(string currentProcess) {

}


int Jenna::decideActionParticular(string action) {
	if (action == "wait_Runway") {
		actionArgs.push_back("waiting.");
		return acaoNula;
	}

	return descansar;
}