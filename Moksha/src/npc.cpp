#include "npc.h"
#include <stdexcept>

NPC::NPC(Map* m, string name) {
	this->mapp = m;
	busy = false;

	FileDict fileObject = FileManager::readFromFile("files/characters/" + name + ".txt");
	this->name = name;
	this->description = new string(concatStrings(editVector("", splitString(fileObject.getValue("description"), '@'), "\n"), 0));
	this->gender = fileObject.getValue("gender")[0];
	this->strength = stoi(fileObject.getValue("strength"));
	this->dexterity = stoi(fileObject.getValue("dexterity"));
	vector<string> v = fileObject.getValues("topics");
	this->topicList = set<string>(v.begin(), v.end());

	goalList = PriorityVector<Goal>(vector<Goal>(), goalCompare);
}


NPC::~NPC() {
	clear();
}

void clearHelper(set<string*> *toClear) {
	for (set<string*>::iterator it = toClear->begin(); it != toClear->end(); it++) {
		delete *it;
	}
	toClear->clear();
}

void NPC::clear() {
	inventory.clear();
	goalList.clear();
	//currentGoal = Goal();
	checkedRooms.clear();
	statusEffects.clear();

	trackablePeople.clear();
	clearHelper(&trackableRooms);
	clearHelper(&trackableConvos);
	clearHelper(&addedConditions);
	clearHelper(&addedActions);

	for (int i = 0; i < conditionNames.size(); i++) {
		delete conditionNames[i];
	}
	conditionNames.clear();
	trackablePeople.clear();
	while (!path.empty())
		path.pop();
	lastSeen.clear();
}


// TRACKERS =====================================================

string* NPC::addTrackable(string trackable, set<string>* trackableSet, string action_name, string condition_name) {
	trackableSet->insert(trackable);
	return addTrackableHelper(action_name, condition_name);
}

string* NPC::addTrackable(string trackable, set<string*>* trackableSet, string action_name, string condition_name) {
	string* new_trackable = new string(trackable);
	trackableSet->insert(new_trackable);
	return addTrackableHelper(action_name, condition_name);
}

string* NPC::addTrackableHelper(string action_name, string condition_name) {
	string* new_action = new string(action_name);
	addedActions.insert(new_action);

	string* new_condition;
	if (condition_name.empty())
		new_condition = new_action;
	else {
		new_condition = new string(condition_name);
		addedConditions.insert(new_condition);
	}

	goap_set_pst(&ap, new_action->c_str(), new_condition->c_str(), true);
	return new_action;
}


void NPC::addTrackableRoom(string room) {
	addTrackable(room, &trackableRooms, "move_" + room, "in_" + room);
}

void NPC::addTrackablePeople(string person) {
	addTrackable(person, &trackablePeople, "search_" + person, "with_" + person);
}

void NPC::addTrackablePeople(string person, vector<string> breadcrumbs) {
	addTrackablePeople(person);
	lastSeen[person].push_back(mapp->getRoom(breadcrumbs[0]));
	for (int i = 0; i < breadcrumbs.size(); i++)
		lastSeen[person].push_back(mapp->getRoom(breadcrumbs[i]));
}

void NPC::addTrackableConvo(string convo) {
	addTrackable(convo, &trackableConvos, "convo_" + convo, "");
}

void NPC::addTrackableConvo(string convo, string precondition) {
	string* convo_novo = addTrackable(convo, &trackableConvos, "convo_" + convo, "");

	set<string*>::iterator it;
	for (it = addedConditions.begin(); it != addedConditions.end(); it++)
		if (**it == precondition)
			break;

	if (it != addedConditions.end())
		goap_set_pre(&ap, convo_novo->c_str(), (*it)->c_str(), true);
}

void NPC::addTrackableNull(string action, string condition, string description, string room) {
	string* new_action = addTrackableHelper(action, condition);
	trackableNulls[action] = tuple<string, string>(condition, description);

	set<string*>::iterator it;
	for (it = addedConditions.begin(); it != addedConditions.end(); it++)
		if (**it == ("in_" + room))
			break;

	if (it != addedConditions.end())
		goap_set_pre(&ap, new_action->c_str(), (*it)->c_str(), true);
}

// PATHFINDING ==========================================

queue<Room*> NPC::findPath(Room* targetRoom) {
	return findPath(currentRoom, targetRoom);
};

queue<Room*> NPC::findPath(Room* initialRoom, Room* targetRoom) {
	return mapp->optimalPath(initialRoom, targetRoom);
};

queue<Room*> NPC::search() {
	return search(vector<Room*>({ currentRoom }));
};

queue<Room*> NPC::search(vector<Room*> breadcrumbs) {
	queue<Room*> retorno;
	retorno.push(currentRoom);

	// Breadcrumbs
	queue<Room*> breadHunt;
	for (vector<Room*>::iterator it = breadcrumbs.begin(); it != breadcrumbs.end(); it++) {
		if (*it != currentRoom) {
			breadHunt = findPath(retorno.back(), *it);
			while (!breadHunt.empty()) {
				retorno.push(breadHunt.front());
				breadHunt.pop();
			}
		}
	}

	// Breadth search
	queue<Room*> search = mapp->breadthSearch(retorno.back());
	while (!search.empty()) {
		queue<Room*> path = findPath(retorno.back(), search.front());
		while (!path.empty()) {
			retorno.push(path.front());
			path.pop();
		}
		search.pop();
	}

	if (retorno.front() == currentRoom)
		retorno.pop();
	return retorno;
};


void NPC::advancePath() {
	if (path.empty())
		setupProcess(plan[currentStep]); // Refresh plans
	else
		path.pop();
}

void NPC::move(string room) {
	Character::move(room);
	advancePath();
}

void NPC::talk(string convo, bool isReaction) {
	for (set<string*>::iterator it = addedConditions.begin(); it != addedConditions.end(); it++)
		if (**it == "convo_" + convo)
			goap_worldstate_set(&ap, &world, (*it)->c_str(), true);
	Character::talk(convo, isReaction);
}

// REACTIONS ==========================================

void NPC::executeReaction(string topic, string phrase, Character* sender, bool shouldRespond) {
	if (isUnconscious())
		return;

	if (topic != "")
		setCondition(topic, true);
	// React to mentions
	if (shouldRespond && !busy && !inConversation()) {
		if (topicList.count(topic) == 0)
			return;
		talk(topic + "_" + name, true);
		notify(avancar);
	}
}


void NPC::setCondition(string condition, bool update) {
	if (!hasCondition(condition)) {
		string *pointer = new string(condition);
		addedConditions.insert(pointer);
		goap_worldstate_set(&ap, &world, pointer->c_str(), update);
	}
	else
		goap_worldstate_set(&ap, &world, condition.c_str(), update);
}


void NPC::receiveEvent(vector<string> args) {
	if (args[0] == "attack")
		if (trackablePeople.count(args[1]) > 0)
			setCondition(args[1] + "_dead", true);
}


void NPC::checkRoomParticular(vector<Character*> peopleInRoom) {
	// Run through tracked people
	for (set<string>::iterator it = trackablePeople.begin(); it != trackablePeople.end(); it++) {
		string theirName = *it;
		goap_worldstate_set(&ap, &world, ("with_" + theirName).c_str(), false);

		// Check if they are in the room
		for (int i = 0; i < peopleInRoom.size(); i++) {
			if (theirName == peopleInRoom[i]->getName()) {
				updateLastSeen(theirName, currentRoom->getCodename());
				goap_worldstate_set(&ap, &world, ("with_" + theirName).c_str(), true);
				break;
			}
		}
	}

	updateWorld();
};


void NPC::seeCharMovingParticular(Character* person, Room* otherRoom, bool entering) {
	if (trackablePeople.count(person->getName()) == 0)
		return;

	if (!entering)
		updateLastSeen(person->getName(), otherRoom->getCodename());
	else
		updateLastSeen(person->getName(), currentRoom->getCodename());

	goap_worldstate_set(&ap, &world, ("with_" + person->getName()).c_str(), entering);
	updateWorld();
}


// PLANS ==================================================

// SETUP -----------------------------
void NPC::setupWorld() {
	goap_actionplanner_clear(&ap); // initializes action planner

	// describe repertoire of actions
	addTrackableRoom("DiningHall");
	addTrackableNull("lunch", "having_lunch", "having lunch", "DiningHall");
	addTrackableNull("dinner", "having_dinner", "having dinner", "DiningHall");
	setupActionsParticular();

	// describe current world state.
	goap_worldstate_clear(&world);
	for (set<string>::iterator it = trackablePeople.begin(); it != trackablePeople.end(); it++)
		setCondition("with_" + *it, false);
	for (set<string*>::iterator it = trackableConvos.begin(); it != trackableConvos.end(); it++)
		setCondition("convo_" + **it, false);
	for (map<string, tuple<string, string>>::iterator it = trackableNulls.begin(); it != trackableNulls.end(); it++)
		setCondition(get<0>((*it).second), false);
	setupWorldParticular();
	updateWorldVariables();

	// Describe default goal
	currentGoal = Goal(0, false);
	goap_worldstate_clear(&currentGoal.goal);
	setupObjectivesParticular();
	goalList.push(currentGoal);
	goalList.sort();

	// calculate initial plan
	createPlan();
}


void NPC::createPlan() {
	plansz = 16;
	stateZero = world;
	planCost = astar_plan(&ap, world, currentGoal.goal, plan, states, &plansz);
	currentStep = -1;
	advancePlans();
}


void NPC::addGoal(vector<string*> conditions, vector<bool> conditionStates, int priority) {
	if (conditions.size() != conditionStates.size())
		return;
	Goal newgoal = Goal(priority, true);
	goap_worldstate_clear(&newgoal.goal);
	for (int i = 0; i < conditions.size(); i++) {
		goap_worldstate_set(&ap, &newgoal.goal, conditions[i]->c_str(), conditionStates[i]);
		conditionNames.push_back(conditions[i]);
	}
	goalList.push(newgoal);
}

void NPC::addGoal(string* condition, bool conditionState, int priority) {
	Goal newgoal = Goal(priority, true);
	goap_worldstate_clear(&newgoal.goal);
	goap_worldstate_set(&ap, &newgoal.goal, condition->c_str(), conditionState);
	conditionNames.push_back(condition);
	goalList.push(newgoal);
}

void NPC::addGoal(bfield_t values, bfield_t dontcare, bool conditionState, int priority) {
	Goal newgoal = Goal(priority, true);
	goap_worldstate_clear(&newgoal.goal);
	newgoal.setGoal(values, dontcare);
	goalList.push(newgoal);
}


// EXTERNAL UPDATE -----------------------------
void NPC::updateWorld() {
	updateWorldVariables();

	// Detect that current action is impossible
	if (plansz > 0) {
		bfield_t prereqs;
		goap_get_pre(&ap, plan[currentStep], &prereqs);
		if (prereqs & world.values != prereqs)
			changePlans(true);
	}
}

void NPC::updateWorldVariables() {
	for (set<string*>::iterator it = trackableRooms.begin(); it != trackableRooms.end(); it++)
		goap_worldstate_set(&ap, &world, ("in_" + **it).c_str(), currentRoom->getCodename() == **it);

	for (map<string, vector<Room*>> ::iterator it = lastSeen.begin(); it != lastSeen.end(); it++)
		goap_set_cost(&ap, ("search_" + (*it).first).c_str(), pathSize( currentRoom, (*it).second[0] ));

	updateWorldExtra();
}


void NPC::updateLastSeen(string pursueTarget, string room) {
	if (!lastSeen.count(pursueTarget))
		lastSeen[pursueTarget] = vector<Room*>({ mapp->getRoom(room) });
	else
		lastSeen[pursueTarget].insert(lastSeen[pursueTarget].begin(), mapp->getRoom(room));

	if (plansz == 0)
		return;

	string currentProcess = plan[currentStep];
	if (currentProcess.compare("search_" + pursueTarget) == 0)
		path = search(lastSeen[pursueTarget]);
}


// ADVANCEMENT ----------------------
void NPC::changePlans(bool justUpdated) {
	if (!justUpdated)
		updateWorldVariables();

	// Calculate a new plan
	createPlan();
}


void NPC::advancePlans() {
	currentStep++;
	// Current plan still going
	if (plansz > 0)
		if (currentStep < plansz)
			setupProcess(plan[currentStep]);

		// End of the line
		else
			changePlans();
}


void NPC::setupProcess(string currentProcess) {
	// Search - Setup path plan
	if (currentProcess.substr(0, 7).compare("search_") == 0) {
		if (lastSeen.count(currentProcess.substr(7, 10000)))
			path = search(lastSeen[currentProcess.substr(7, 10000)]);
		else
			path = search();
	}
	// Move - setup path plan
	else if (currentProcess.substr(0, 5).compare("move_") == 0) {
		path = findPath(mapp->getRoom(currentProcess.substr(5, 10000)));
	}
	// Other actions that require prior planning
	else
		setupProcessParticular(currentProcess);
}


// ACTIONS =======================================

int NPC::decideAction() {
	goalList.sort();
	// Current objective is not top priority - find next non-completed objective
	if (currentGoal.goal.values != goalList.highest()->goal.values) {
		vector<Goal>::iterator it = goalList.highest();
		do {
			currentGoal = *it;
			currentStep = 0;
			goalList.descend(it);
		} while ((world.values & ~currentGoal.goal.dontcare) == currentGoal.goal.values && it != goalList.lowest());
		changePlans();

	// Current objective is top priority
	} else {
		// Current objective was completed - find next non-completed objective
		vector<Goal>::iterator it = goalList.highest();
		if ( ((world.values & ~currentGoal.goal.dontcare) == currentGoal.goal.values || currentStep >= plansz) && !goalList.empty() ) {
			do {
				if (it->onetime) {
					vector<Goal>::iterator dit = it;
					goalList.descend(it);
					goalList.erase(dit);
				} else
					goalList.descend(it);

				currentGoal = *it;
				currentStep = 0;
			} while ( (world.values & ~currentGoal.goal.dontcare) == currentGoal.goal.values && it != goalList.lowest() );
			changePlans();
		}
		// Current objective not completed - find next action
		else {
			while (isCurrentStateFulfilled() && plansz > 0)
				advancePlans();
		}
	}

	// Decide action
	if (plansz > 0) {
		actionArgs.clear();
		string action = plan[currentStep];

		// Move / Search
		if (action.substr(0, 5).compare("move_") == 0 || action.substr(0, 7).compare("search_") == 0) {
			actionArgs.push_back(path.front()->getCodename());
			currentAction = mover;
		}

		// Convo
		else if (action.substr(0, 6).compare("convo_") == 0) {
			actionArgs.push_back(action.substr(6, 1000));
			currentAction = conversar;

		// Null action
		} else if (trackableNulls.count(action)) {
			actionArgs.push_back(get<1>(trackableNulls[action]) + ".");
			currentAction = acaoNula;

		// Other actions
		} else 
			currentAction = decideActionParticular(action);
	} else
		currentAction = descansar;

	busy = !(currentAction == descansar || currentAction == acaoNula);
	return currentAction;
}


// HELPER =========================================

bool NPC::isCurrentStateFulfilled() {
	bfield_t worldState = (world.values & ~states[currentStep].dontcare);
	bfield_t objState = states[currentStep].values;
	if (currentStep > 0)
		objState = states[currentStep].values & ~states[currentStep-1].values;
	else
		objState = states[currentStep].values & ~stateZero.values;
	bool fulfilled = ((worldState & objState) == objState);
	if (fulfilled) {
		if (currentStep > 0)
			objState = states[currentStep].values | ~states[currentStep - 1].values;
		else
			objState = states[currentStep].values | ~stateZero.values;
		fulfilled = fulfilled && ((worldState | objState) == objState);
	}

	return fulfilled;
}


bool NPC::hasCondition(string info) {
	bool retorno;
	if (!goap_worldstate_get(&ap, &world, info.c_str(), &retorno))
		return false;
	return retorno;
}


// GETTER ====================================
vector<string> NPC::getActionList() {
	vector<string> retorno;
	for (int i = 0; i < ap.numactions; i++)
		retorno.push_back(ap.act_names[i]);
	return retorno;
}


vector<string> NPC::getAtomList() {
	vector<string> retorno;
	for (int i = 0; i < ap.numatoms; i++)
		retorno.push_back(ap.atm_names[i]);
	return retorno;
}


// EXTRA =================================

void NPC::setupCrewArea() {
	addTrackableRoom("Mezzanine");
	addTrackableRoom("CrewCorridor");
	goap_set_pre(&ap, "enter_CrewArea", "in_Mezzanine", true);
	goap_set_pst(&ap, "enter_CrewArea", "in_CrewArea", true);
	goap_set_pre(&ap, "leave_CrewArea", "in_CrewCorridor", true);
	goap_set_pst(&ap, "leave_CrewArea", "in_CrewArea", false);
}