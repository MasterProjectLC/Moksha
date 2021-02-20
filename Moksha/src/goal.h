#pragma once
#include "../libs/GPGOAP/goap.h"

using namespace std;

class Goal {
public:
	worldstate_t goal;
	int priority;
	bool onetime;
};

static bool goalCompare(Goal a, Goal b) { return a.priority < b.priority; }