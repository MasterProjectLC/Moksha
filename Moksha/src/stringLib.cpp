#include "stringLib.h"

bool stringEqual(string str1, string str2) {
	return str1.compare(0, str2.length() - 2, str2) == 0;
}

vector<string> splitString(string str, char delim) {
	vector<string> retorno;
	
	size_t current, previous = 0;
	current = str.find(delim);
	while (current != string::npos) {
		retorno.push_back(str.substr(previous, current - previous));
		previous = current + 1;
		current = str.find(delim, previous);

	}

	retorno.push_back(str.substr(previous, current - previous));
	return retorno;
}

set<string> splitStringSet(string str, char delim) {
	set<string> retorno;

	size_t current, previous = 0;
	current = str.find(delim);
	while (current != string::npos) {
		retorno.insert(str.substr(previous, current - previous));
		previous = current + 1;
		current = str.find(delim, previous);

	}

	retorno.insert(str.substr(previous, current - previous));
	return retorno;
}

string concatStrings(vector<string> args, int inicio) {
	string r = args.at(inicio);
	for (int i = inicio + 1; i < args.size(); i++) {
		r += " " + args.at(i);
	}
	return r;
}