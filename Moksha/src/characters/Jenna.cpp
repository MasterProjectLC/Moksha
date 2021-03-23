#include "Jenna.h"

Jenna::Jenna(Map* m) : NPC{m, "Jenna", 
("Jenna Maybourne, George's niece, is an american 'innovation journalist', a title she coined herself. "
"She's also my boss. Overall, I find her somewhat laidback and playful, simultaneously taking her job seriously and having fun along the way. "
"Her connection to her father, who was a prominent aeronaut, sparked her interest for technological marvels and allowed her to learn a lot about airships.\n"
"She's currently wearing a brown vest, a bowler hat and big, round googles. Her hair is shorter than most women."),
F, 9, 10} {
	trackablePeople.insert("Elliot");
	trackablePeople.insert("Santos");
}

void Jenna::setupActionsParticular() {
	goap_set_pst(&ap, "move_kitchen", "in_kitchen", true);

	goap_set_pre(&ap, "take_knife", "in_kitchen", true);
	goap_set_pst(&ap, "take_knife", "armed", true);

	goap_set_pre(&ap, "search_Elliot", "armed", true);
	goap_set_pre(&ap, "search_Elliot", "Elliot_dead", false);
	goap_set_pst(&ap, "search_Elliot", "with_Elliot", true);
	//goap_set_cost(&ap, "search_Elliot", 5);

	goap_set_pre(&ap, "kill_Elliot", "armed", true);
	goap_set_pre(&ap, "kill_Elliot", "with_Elliot", true);
	goap_set_pst(&ap, "kill_Elliot", "Elliot_dead", true);

	goap_set_pre(&ap, "search_Santos", "armed", true);
	goap_set_pre(&ap, "search_Santos", "Santos_dead", false);
	goap_set_pst(&ap, "search_Santos", "with_Santos", true);
	//goap_set_cost(&ap, "search_Elliot", 5);

	goap_set_pre(&ap, "kill_Santos", "armed", true);
	goap_set_pre(&ap, "kill_Santos", "with_Santos", true);
	goap_set_pst(&ap, "kill_Santos", "Santos_dead", true);
}


void Jenna::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "with_Elliot", false);
	goap_worldstate_set(&ap, &world, "Elliot_dead", false);
	goap_worldstate_set(&ap, &world, "with_Santos", false);
	goap_worldstate_set(&ap, &world, "Santos_dead", false);
	updateWorldExtra();
}


void Jenna::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "Elliot_dead", true);

	Goal killSantos = Goal(1, true);
	goap_worldstate_clear(&killSantos.goal);
	goap_worldstate_set(&ap, &killSantos.goal, "Santos_dead", true);
	goalList.push(killSantos);
}


void Jenna::updateWorldExtra() {
	// describe current world state.
	goap_worldstate_set(&ap, &world, "in_kitchen", currentRoom->getName() == "Kitchen");
	goap_worldstate_set(&ap, &world, "armed", inventory.hasItem("Knife"));

	goap_set_cost(&ap, "move_kitchen", pathSize(currentRoom, mapp->getRoom("Kitchen")));
	if (lastSeen.hasKey("Elliot"))
		goap_set_cost(&ap, "search_Elliot", pathSize(currentRoom, mapp->getRoom( lastSeen.getValues("Elliot") )));

	if (lastSeen.hasKey("Santos"))
		goap_set_cost(&ap, "search_Santos", pathSize(currentRoom, mapp->getRoom( lastSeen.getValues("Santos") )));
}


int Jenna::decideActionParticular(string action) {
	if (action == "move_kitchen" || action.substr(0, 7).compare("search_") == 0) {
		actionArgs.push_back(nextRoomInPath());
		return mover;
	}

	else if (action == "take_knife") {
		actionArgs.push_back("take");
		actionArgs.push_back("Knife");
		return interagir;
	}

	else if (action.substr(0, 5).compare("kill_") == 0) {
		actionArgs.push_back(action.substr(5, 1000));
		return atacar;
	}

	return descansar;
}


void Jenna::advancePlansExtra(string currentProcess) {
	if (currentProcess == "move_kitchen")
		path = findPath(mapp->getRoom("Kitchen"));
}