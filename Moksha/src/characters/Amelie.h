#pragma once
#include "../npc.h"

using namespace std;

class Amelie : public NPC {
private:

public:
	Amelie() : NPC{ NULL, "Amelie", "", F, 0, 0 } {}
	explicit Amelie(Map* m);
	int decideActionParticular(string action) override;
	void advancePlansExtra(string currentProcess) override;
	void setupWorldParticular() override;
	void setupObjectivesParticular() override;
	void updateWorldExtra() override;
	void setupActionsParticular() override;
};