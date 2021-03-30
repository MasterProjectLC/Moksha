#include "concept.h"


Concept::Concept(string name, string description) {
	this->name = name;
	this->codename = name;
	this->description = description;
}

Concept::Concept(string name, string codename, string description) {
	this->name = name;
	this->codename = codename;
	this->description = description;
}