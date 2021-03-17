#pragma once
#include "../libs/GPGOAP/goap.h"

using namespace std;

class Goal {
public:
	Goal() {}
	Goal(int priority, bool onetime) {
		this->priority = priority;
		this->onetime = onetime;
	}

	worldstate_t goal;
	int priority;
	bool onetime;
};

static bool goalCompare(Goal a, Goal b) { return a.priority < b.priority; }