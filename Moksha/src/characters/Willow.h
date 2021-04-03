#pragma once
#include "../npc.h"

using namespace std;

class Willow : public NPC {
private:

public:
	Willow() : NPC{ NULL, "Willow", "", F, 1, 1 } {}
	explicit Willow(Map* m);
	int decideActionParticular(string action) override;
	void updateProcessExtra(string currentProcess) override;
	void setupWorldParticular() override;
	void setupObjectivesParticular() override;
	void updateWorldExtra() override;
	void setupActionsParticular() override;
};