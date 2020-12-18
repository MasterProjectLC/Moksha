#include "fileDict.h"

FileDict::FileDict(string fileText) {
	vector<string> linhas = splitString(fileText, '\n');
	for (int i = 0; i < linhas.size(); i++) {
		// Found a Key-Value pair
		if (!linhas[i].empty() && linhas[i].at(linhas[i].size()-1) == ':') {
			// Insert key
			keys.push_back(splitString(linhas[i].substr(0, linhas[i].size()-1), ','));

			// Insert value
			string s("");
			for (i++; (i < linhas.size()) && !(!linhas[i].empty() && linhas[i].at(linhas[i].size()-1) == ':'); i++) {
				s.append(linhas[i]);
			}
			values.push_back(splitString(s, '|'));
			i--;
		}
	}
};

string FileDict::getValue(string key) {
	for (int i = 0; i < keys.size(); i++)
		for (int j = 0; j < keys[i].size(); j++)
			if (keys[i][j].compare(key) == 0)
				return values[i][0];

	return "teste";
}

vector<string> FileDict::getValues(string key) {
	for (int i = 0; i < keys.size(); i++)
		for (int j = 0; j < keys[i].size(); j++)
			if (keys[i][j].compare(key) == 0 && i < values.size())
				return values[i];

	vector<string> a;
	return a;
}