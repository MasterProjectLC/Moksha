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
	addTrackableRoom("ViewingLobby");

	addTrackableRoom("JennaRoom");
	goap_set_pre(&ap, "write", "in_JennaRoom", true);
	goap_set_pst(&ap, "write", "writing", true);

	addTrackableConvo("wake_up");
	goap_set_pre(&ap, "convo_wake_up", "in_JennaRoom", true);

	addTrackableRoom("Runway");
	goap_set_pre(&ap, "wait_Runway", "in_Runway", true);
	goap_set_pst(&ap, "wait_Runway", "waiting_Runway", true);
}


void Jenna::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "waiting_Runway", false);
	goap_worldstate_set(&ap, &world, "writing", false);
}


void Jenna::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "writing", true);
	addGoal(new string("convo_wake_up"), true, 5);
}


void Jenna::updateWorldExtra() {

}


void Jenna::setupProcessParticular(string currentProcess) {

}


int Jenna::decideActionParticular(string action) {
	if (action == "wait_Runway") {
		actionArgs.push_back("waiting.");
		return acaoNula;
	}
	else if (action == "write") {
		actionArgs.push_back("writing.");
		return acaoNula;
	}

	return descansar;
}