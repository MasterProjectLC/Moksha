#pragma once
#include <vector>
#include <string>
#include "object.h"

using namespace std;

class Room {
	// Atributos
private:
	string name;
	string codename;
	string initialText;
	vector<string> adjacentRoomCodenames;
	int index;

	vector<string> objectNames;
	vector<Object> objects;

public:
	Room() {};

	// Setup
	Room(int index, string name, string initialText, vector<string> adjacentRoomNames);
	Room(int index, string name, string codename, string initialText, vector<string> adjacentRoomNames);
	void setObjectNames(vector<string> objectNames) { this->objectNames = objectNames; }

	void setupObjects(vector<Object> objects) { this->objects = objects; }
	void addObject(Object object) { objects.push_back(object); }
	void removeObject(Object object);
	void clearObjects() { objects.clear(); objectNames.clear(); }

	// Getters
	string getName() { return name; }
	string getCodename() { return codename; }
	string getInitialText() { return initialText; }
	int getIndex() { return index; }

	bool isRoomAdjacent(string adjacentRoomName);
	string getAdjacentRoomCodename(int i);
	int getAdjacentRoomCount() { return adjacentRoomCodenames.size(); }

	bool hasObject(string name);
	Object* getObject(string name);
	Object* getObject(int index) { return &objects[index]; }
	vector<Object> getObjects() { return objects; }
	vector<string> getObjectNames();

};