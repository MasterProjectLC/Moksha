#include "room.h"

Room::Room(int index, string name, string initialText, vector<string> adjacentRoomCodenames) {
	this->index = index;
	this->name = name;
	this->codename = name;
	this->initialText = initialText;
	this->adjacentRoomCodenames = adjacentRoomCodenames;
}

Room::Room(int index, string name, string codename, string initialText, vector<string> adjacentRoomCodenames) {
	this->index = index;
	this->name = name;
	this->codename = codename;
	this->initialText = initialText;
	this->adjacentRoomCodenames = adjacentRoomCodenames;
}


void Room::removeObject(Object object) {
	bool found = false;
	for (int i = 0; i < objects.size(); i++) {
		if (found) {
			objects[i].setNotifyID(objects[i].getNotifyID()-1);
			objects[i-1] = objects[i];
			objectNames[i - 1] = objectNames[i];

		} else if (objects[i].getName().compare(object.getName()) == 0) {
			found = true;
		}
	}

	if (found) {
		objects.resize(objects.size() - 1);
		objectNames.resize(objectNames.size() - 1);
	}
}

bool Room::isRoomAdjacent(string adjacentRoomCodename) {
	for (int i = 0; i < adjacentRoomCodenames.size(); i++)
		if (adjacentRoomCodename == adjacentRoomCodenames[i])
			return true;
	return false;
}

// GETTERS
string Room::getAdjacentRoomCodename(int i) {
	return adjacentRoomCodenames[i];
};


bool Room::hasObject(string name) {
	for (int i = 0; i < objects.size(); i++) {
		if (name.compare(objects[i].getName()) == 0)
			return true;
	}
	return false;
}


#include <stdexcept>
Object* Room::getObject(string name) {
	for (int i = 0; i < objects.size(); i++) {
		if (name.compare(objects[i].getName()) == 0)
			return &objects[i];
	}

	throw invalid_argument("There's no object with that name :(");
}


vector<string> Room::getObjectNames() {
	return objectNames;
};
