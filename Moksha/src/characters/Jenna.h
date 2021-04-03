#pragma once
#include "../npc.h"

using namespace std;

class Jenna: public NPC {
private:

public:
	Jenna() : NPC{ NULL, "Jenna", "", F, 7, 1 } {}
	explicit Jenna(Map* m);
	int decideActionParticular(string action) override;
	void updateProcessExtra(string currentProcess) override;
	void setupWorldParticular() override;
	void setupObjectivesParticular() override;
	void updateWorldExtra() override;
	void setupActionsParticular() override;
};