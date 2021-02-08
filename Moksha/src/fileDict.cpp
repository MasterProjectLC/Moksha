#include "fileDict.h"

FileDict::FileDict(string fileText) {
	vector<string> linhas = splitString(fileText, '\n');
	for (int i = 0; i < linhas.size(); i++) {
		// Found a Key-Value pair
		if (!linhas[i].empty() && linhas[i].at(linhas[i].size()-1) == ':') {
			// Insert key
			set<string> key = splitStringSet(linhas[i].substr(0, linhas[i].size()-1), ',');

			// Insert value
			string s("");
			for (i++; (i < linhas.size()) && !(!linhas[i].empty() && linhas[i].at(linhas[i].size()-1) == ':'); i++) {
				s.append(linhas[i]);
			}
			addPair(key, splitString(s, '|'));
			i--;
		}
	}
};