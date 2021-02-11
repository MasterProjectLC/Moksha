#include "npc.h"

NPC::NPC(Mapa* m, string name, int genero, int forca, int destreza) : Personagem(genero, forca, destreza) {
	this->nome = name;
	this->mapa = m;

	FileDict fileObjeto = FileManager::readFromFile("files/personagens/" + getNome() + ".txt");
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

// ----------------------------------

void NPC::executarReacao(string topico, string frase, string remetente) {
	if (isInconsciente())
		return;

	if (dict.hasKey(topico))
		say(topico, dict.getValue(topico), set<string>({ remetente }));
}


void NPC::verSala(vector<Personagem*> pessoasNaSala) {
	goap_worldstate_set(&ap, &current, "com_alvo", false);

	for (int i = 0; i < alvos.size(); i++) {
		goap_worldstate_set(&ap, &current, ("com_" + alvos[i]).c_str(), false);
		for (int i = 0; i < pessoasNaSala.size(); i++)
			goap_worldstate_set(&ap, &current, ("com_" + alvos[i]).c_str(), true);
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

		goap_worldstate_set(&ap, &current, ("com_" + alvos[idx]).c_str(), entrando);
		updatePlanos();
	}
}


// PLANOS -------------------------------------------

void NPC::setupPlanos() {
	goap_actionplanner_clear(&ap); // initializes action planner
	vector<string> nomes = { "Ned", "Baxter", "Willow", "Hilda", "Santos",
						"Magnus", "Tom", "Jenna", "Renard", "Liz",
						"George", "Damian", "Amelie" };

	// describe repertoire of actions
	setupAcoesAdicional();
	goap_set_pst(&ap, "descansar", "descansado", true);	// Acao dummy para nao fazer nada caso objetivos estejam vazios
	goap_set_cost(&ap, "descansar", 100);
	for (int i = 0; i < nomes.size(); i++) {
		goap_set_pst(&ap, ("procurar_" + nomes[i]).c_str(), ("com_" + nomes[i]).c_str(), true);
	}

	// describe current world state.
	goap_worldstate_clear(&current);
	goap_worldstate_set(&ap, &current, "vivo", true);
	goap_worldstate_set(&ap, &current, "descansado", false);
	for (int i = 0; i < nomes.size(); i++) {
		goap_worldstate_set(&ap, &current, (nomes[i] + "_vivo").c_str(), true);
		if (nomes[i] != nome)
			goap_worldstate_set(&ap, &current, ("com_" + nomes[i]).c_str(), false);
		else
			goap_worldstate_set(&ap, &current, ("com_" + nomes[i]).c_str(), true);
	}

	goap_worldstate_set(&ap, &current, "armado", inventario.temItem("Faca"));

	setupMundoAdicional();

	// describe goal
	goap_worldstate_clear(&goal);
	goap_worldstate_set(&ap, &goal, "descansado", true);
	setupObjetivosAdicional();

	planCost = astar_plan(&ap, current, goal, plan, states, &plansz);
	currentStep = -1;
	avancarPlanos();
}

bool NPC::temCondicao(string info) {
	bool retorno;
	if (!goap_worldstate_get(&ap, &current, info.c_str(), &retorno))
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