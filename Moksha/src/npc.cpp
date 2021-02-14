#include "npc.h"

NPC::NPC(Mapa* m, string name, int genero, int forca, int destreza) : Personagem(genero, forca, destreza) {
	this->nome = name;
	this->mapa = m;

	FileDict fileObjeto = FileManager::readFromFile("files/characters/" + getNome() + ".txt");
	this->dict = fileObjeto;
}


// PATHFINDING ---------------

queue<Sala*> NPC::descobrirCaminho(Sala* salaAlvo) {
	return descobrirCaminho(salaAtual, salaAlvo);
};

queue<Sala*> NPC::descobrirCaminho(Sala* salaInicial, Sala* salaAlvo) {
	return mapa->optimalPath(salaInicial, salaAlvo);
};

queue<Sala*> NPC::procurar() {
	return procurar(salaAtual);
};

queue<Sala*> NPC::procurar(Sala* salaPista) {
	queue<Sala*> retorno;
	retorno.push(salaAtual);
	if (salaPista != salaAtual)
		retorno = descobrirCaminho(salaPista);
	queue<Sala*> procura = mapa->breadthSearch(salaPista);
	while (!procura.empty()) {
		queue<Sala*> caminho = descobrirCaminho(retorno.back(), procura.front());
		while (!caminho.empty()) {
			retorno.push(caminho.front());
			caminho.pop();
		}
		procura.pop();
	}

	retorno.pop();
	return retorno;
};


void NPC::seguirCaminho() {
	if (!caminho.empty()) {
		move(*caminho.front());
		caminho.pop();
	}
}

// REACÕES ----------------------------------

void NPC::executarReacao(string topico, string frase, string remetente) {
	if (isUnconscious())
		return;

	if (dict.hasKey(topico))
		say(topico, dict.getValue(topico), set<string>({ remetente }));
}


void NPC::verSala(vector<Personagem*> pessoasNaSala) {
	goap_worldstate_set(&ap, &world, "with_target", false);

	for (int i = 0; i < alvos.size(); i++) {
		goap_worldstate_set(&ap, &world, ("with_" + alvos[i]).c_str(), false);
		for (int i = 0; i < pessoasNaSala.size(); i++)
			goap_worldstate_set(&ap, &world, ("with_" + alvos[i]).c_str(), true);
		updatePlanos();
		break;
	}

};


void NPC::verPessoaMovendo(Personagem* pessoa, string outraSala, bool entrando) {
	int idx = alvoIndex(pessoa->getNome());

	if (idx != -1) {
		if (!entrando) {
			string alvoVec[1] = { alvos[idx] };
			ultimoAvistamento.addPair(set<string>(alvoVec, alvoVec+1), outraSala);
		}

		goap_worldstate_set(&ap, &world, ("with_" + alvos[idx]).c_str(), entrando);
		updatePlanos();
	}
}


// PLANOS E AÇÕES -------------------------------------------

void NPC::decidirAcao() {
	if (currentStep >= plansz)
		updatePlanos();

	if (plansz > 0)
		acaoAtual = plan[currentStep];
	else
		acaoAtual = "";
}


void NPC::tomarAcao() {
	// Trancado em conversa
	if (inConversation()) {
		setInConversation(false);
		return;
	}

	tomarAcaoParticular(acaoAtual);
}


void NPC::setupPlanos() {
	goap_actionplanner_clear(&ap); // initializes action planner
	vector<string> nomes = { "Elliot", "Baxter", "Willow", "Hilda", "Santos",
								"Magnus", "Tom", "Jenna", "Renard", "Liz",
								"George", "Damian", "Amelie" };

	// describe repertoire of actions
	setupAcoesAdicional();
	for (int i = 0; i < nomes.size(); i++) {
		goap_set_pst(&ap, ("search_" + nomes[i]).c_str(), ("with_" + nomes[i]).c_str(), true);
	}

	// describe current world state.
	goap_worldstate_clear(&world);
	goap_worldstate_set(&ap, &world, "alive", true);
	for (int i = 0; i < nomes.size(); i++) {
		goap_worldstate_set(&ap, &world, (nomes[i] + "_alive").c_str(), true);
		if (nomes[i] != nome)
			goap_worldstate_set(&ap, &world, ("with_" + nomes[i]).c_str(), false);
		else
			goap_worldstate_set(&ap, &world, ("with_" + nomes[i]).c_str(), true);
	}

	goap_worldstate_set(&ap, &world, "armed", inventario.temItem("Knife"));

	setupMundoAdicional();

	// describe goal
	goap_worldstate_clear(&currentGoal.goal);
	setupObjetivosAdicional();
	goalList.push(currentGoal);

	// calculate initial plan
	planCost = astar_plan(&ap, world, currentGoal.goal, plan, states, &plansz);
	currentStep = -1;
	avancarPlanos();
}


void NPC::updatePlanos() {
	// describe current world state.
	updatePlanosAdicional();

	planCost = astar_plan(&ap, world, currentGoal.goal, plan, states, &plansz);
	currentStep = -1;
	avancarPlanos();
}


bool NPC::temCondicao(string info) {
	bool retorno;
	if (!goap_worldstate_get(&ap, &world, info.c_str(), &retorno))
		return false;
	return retorno;
}

void NPC::avancarPlanos() {
	currentStep++;

	if (currentStep >= plansz && plansz > 0)
		updatePlanos();

	avancarPlanosAdicional();
}


// HELPER ----------------------------------------

int NPC::alvoIndex(string nome) {
	for (int i = 0; i < alvos.size(); i++) {
		if (nome == alvos[i])
			return i;
	}

	return -1;
}