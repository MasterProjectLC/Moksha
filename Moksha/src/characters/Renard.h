#pragma once
#include "../npc.h"

using namespace std;

class Renard : public NPC {
private:

public:
	Renard() : NPC{ NULL, "Renard", "", M, 0, 0 } {}
	explicit Renard(Map* m);
	int decideActionParticular(string action) override;
	void advancePlansExtra(string currentProcess) override;
	void setupWorldParticular() override;
	void setupObjectivesParticular() override;
	void updateWorldExtra() override;
	void setupActionsParticular() override;
};