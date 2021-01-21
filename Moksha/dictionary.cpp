#include "dictionary.h"

string Dictionary::getValue(string key) {
	for (int i = 0; i < keys.size(); i++)
		for (int j = 0; j < keys[i].size(); j++)
			if (keys[i][j].compare(key) == 0)
				return values[i][0];

	return "teste";
}

vector<string> Dictionary::getValues(string key) {
	for (int i = 0; i < keys.size(); i++)
		for (int j = 0; j < keys[i].size(); j++)
			if (keys[i][j].compare(key) == 0 && i < values.size())
				return values[i];

	vector<string> a;
	return a;
}