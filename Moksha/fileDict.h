#pragma once
#include "dictionary.h"

using namespace std;

class FileDict: public Dictionary<vector<string>> {
public:
	FileDict(string fileText);

	string getValue(string key) {
		for (int i = 0; i < keys.size(); i++)
			for (int j = 0; j < keys[i].size(); j++)
				if (keys[i][j].compare(key) == 0)
					return values[i][0];

		return "teste";
	}
};