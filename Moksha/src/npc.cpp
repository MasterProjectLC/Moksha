#include "npc.h"

NPC::NPC(Mapa* m, string name, int gender, int forca, int destreza) : Personagem(gender, forca, destreza) {
	this->nome = name;
	this->mapa = m;

	FileDict fileObjeto = FileManager::readFromFile("files/characters/" + getNome() + ".txt");
	this->dict = fileObjeto;
}


// PATHFINDING ---------------

queue<Sala*> NPC::findPath(Sala* salaAlvo) {
	return findPath(salaAtual, salaAlvo);
};

queue<Sala*> NPC::findPath(Sala* salaInicial, Sala* salaAlvo) {
	return mapa->optimalPath(salaInicial, salaAlvo);
};

queue<Sala*> NPC::search() {
	return search(salaAtual);
};

queue<Sala*> NPC::search(Sala* salaPista) {
	queue<Sala*> retorno;
	retorno.push(salaAtual);
	if (salaPista != salaAtual)
		retorno = findPath(salaPista);
	queue<Sala*> procura = mapa->breadthSearch(salaPista);
	while (!procura.empty()) {
		queue<Sala*> caminho = findPath(retorno.back(), procura.front());
		while (!caminho.empty()) {
			retorno.push(caminho.front());
			caminho.pop();
		}
		procura.pop();
	}

	retorno.pop();
	return retorno;
};


string NPC::nextRoomInPath() {
	string retorno = "";
	if (path.empty())
		advancePlansExtra(plan[currentStep]); // Refresh plans

	if (!path.empty()) {
		retorno = path.front()->getNome();
		path.pop();
	}

	return retorno;
}

// REACÕES ----------------------------------

void NPC::executeReaction(string topico, string frase, string remetente) {
	if (isUnconscious())
		return;

	if (dict.hasKey(topico))
		say(topico, dict.getValue(topico), set<string>({ remetente }));
}


void NPC::checkRoom(vector<Personagem*> pessoasNaSala) {
	for (int i = 0; i < alvos.size(); i++) {
		goap_worldstate_set(&ap, &world, ("with_" + alvos[i]).c_str(), false);
		for (int j = 0; j < pessoasNaSala.size(); j++) {
			if (alvos[i] == pessoasNaSala[j]->getNome()) {
				goap_worldstate_set(&ap, &world, ("with_" + alvos[i]).c_str(), true);
				break;
			}
		}
	}

	updateWorld();
};


void NPC::seeCharMoving(Personagem* pessoa, string outraSala, bool entrando) {
	int idx = alvoIndex(pessoa->getNome());

	if (idx != -1) {
		if (!entrando) {
			string alvoVec[1] = { alvos[idx] };
			ultimoAvistamento.addPair(set<string>(alvoVec, alvoVec+1), outraSala);
			// TODO: criar sistema de ultimo avistamento mais modular e sofisticado
		}
		goap_worldstate_set(&ap, &world, ("with_" + alvos[idx]).c_str(), entrando);
		updateWorld();
	}
}


// PLANOS -------------------------------------------

void NPC::setupPlans() {
	goap_actionplanner_clear(&ap); // initializes action planner

	// describe repertoire of actions
	static vector<string> search_atoms = editVector("search_", nomes, "");
	static vector<string> with_atoms = editVector("with_", nomes, "");
	static vector<string> alive_atoms = editVector("", nomes, "_alive");
	for (int i = 0; i < search_atoms.size(); i++)
		if (nomes[i] != nome) {
			goap_set_pre(&ap, search_atoms[i].c_str(), alive_atoms[i].c_str(), true);
			goap_set_pst(&ap, search_atoms[i].c_str(), with_atoms[i].c_str(), true);
		}
	setupAcoesAdicional();

	// describe current world state.
	goap_worldstate_clear(&world);
	for (int i = 0; i < alive_atoms.size(); i++) {
		goap_worldstate_set(&ap, &world, alive_atoms[i].c_str(), true);
		if (nomes[i] != nome)
			goap_worldstate_set(&ap, &world, ("with_" + nomes[i]).c_str(), false);
	}
	goap_worldstate_set(&ap, &world, "armed", inventario.temItem("Knife"));
	setupMundoAdicional();

	// describe goal
	goap_worldstate_clear(&currentGoal.goal);
	setupObjetivosAdicional();
	goalList = PriorityVector<Goal>(vector<Goal>(), goalCompare);
	goalList.push(currentGoal);

	// calculate initial plan
	planCost = astar_plan(&ap, world, currentGoal.goal, plan, states, &plansz);
	currentStep = -1;
	updateWorld();
	advancePlans();
}


void NPC::updateWorld() {
	updateWorldExtra();

	// Detect that current action is impossible
	if (plansz > 0) {
		bfield_t prereqs;
		goap_get_pre(&ap, plan[currentStep], &prereqs);
		if (prereqs & world.values != prereqs)
			changePlans(true);
	}
}


void NPC::advancePlans() {
	currentStep++;

	// The plan has come to its end
	if (currentStep >= plansz && plansz > 0)
		changePlans();

	// Advance plans
	if (plansz > 0)
		advancePlansExtra(plan[currentStep]);
}


void NPC::changePlans(bool justUpdated) {
	if (!justUpdated)
		updateWorldExtra();

	// Calculate a new plan
	planCost = astar_plan(&ap, world, currentGoal.goal, plan, states, &plansz);
	currentStep = -1;
	advancePlans();
}


// AÇÕES ------------------------------------------------

int NPC::decideAction() {
	// Check if current action was completed
	bool a;
	goap_worldstate_get(&ap, &world, "with_Elliot", &a);
	if ( (world.values & ~states[currentStep].dontcare) == states[currentStep].values )
		advancePlans();

	// Current objective is top priority
	if (currentGoal.goal.values == (goalList.highest()->goal.values)) {
		bool a;
		goap_worldstate_get(&ap, &world, "Elliot_alive", &a);
		goap_worldstate_get(&ap, &world, "with_Elliot", &a);
		// Current objective was completed - find next non-completed objective
		vector<Goal>::iterator it = goalList.highest();
		if ( ((world.values & ~currentGoal.goal.dontcare) == currentGoal.goal.values || currentStep >= plansz) && !goalList.empty() ) {
			do {
				if (it->onetime)
					goalList.getVector().erase(it);

				goalList.descend(it);
				currentGoal = *it;
				currentStep = 0;
			} while ( (world.values & ~currentGoal.goal.dontcare) == currentGoal.goal.values && it != goalList.lowest() );
			changePlans();
		}
	
	// Current objective is not top priority - find next non-completed objective
	} else {
		vector<Goal>::iterator it = goalList.highest();
		while ((world.values & ~currentGoal.goal.dontcare) == currentGoal.goal.values && it != goalList.lowest() ) {
			goalList.descend(it);
			currentGoal = *it;
			currentStep = 0;
		}
		changePlans();
	}

	// Decidir ação
	if (plansz > 0) {
		actionArgs.clear();
		currentAction = decidirAcaoAdicional(plan[currentStep]);
	} else
		currentAction = descansar;

	return currentAction;
}


bool NPC::hasCondition(string info) {
	bool retorno;
	if (!goap_worldstate_get(&ap, &world, info.c_str(), &retorno))
		return false;
	return retorno;
}


// HELPER ----------------------------------------

int NPC::alvoIndex(string nome) {
	for (int i = 0; i < alvos.size(); i++) {
		if (nome == alvos[i])
			return i;
	}

	return -1;
}