#include "concept.h"

Concept::Concept(string name) {
	this->name = name;
	this->description = "";
}

Concept::Concept(string name, string description) {
	this->name = name;
	this->description = description;
}