#include "sala.h"

Sala::Sala(int index, string name, string initialText, vector<string> salasAnexasNomes, vector<string> objectNames) {
	this->index = index;
	this->name = name;
	this->codename = name;
	this->initialText = initialText;
	this->salasAnexasNomes = salasAnexasNomes;
	this->objectNames = objectNames;
}

Sala::Sala(int index, string name, string codename, string initialText, vector<string> salasAnexasNomes, vector<string> objectNames) {
	this->index = index;
	this->name = name;
	this->codename = codename;
	this->initialText = initialText;
	this->salasAnexasNomes = salasAnexasNomes;
	this->objectNames = objectNames;
}


void Sala::removeObject(Object object) {
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

bool Sala::isSalaAnexa(string salaAnexaNome) {
	for (int i = 0; i < salasAnexasNomes.size(); i++)
		if (salaAnexaNome == salasAnexasNomes[i])
			return true;
	return false;
}

// GETTERS
string Sala::getAdjacentRoomName(int i) {
	return salasAnexasNomes[i];
};


bool Sala::hasObject(string name) {
	for (int i = 0; i < objects.size(); i++) {
		if (name.compare(objects[i].getName()) == 0)
			return true;
	}
	return false;
}


#include <stdexcept>
Object* Sala::getObject(string name) {
	for (int i = 0; i < objects.size(); i++) {
		if (name.compare(objects[i].getName()) == 0)
			return &objects[i];
	}

	throw invalid_argument("There's no object with that name :(");
}


vector<string> Sala::getObjectNames() {
	return objectNames;
};
