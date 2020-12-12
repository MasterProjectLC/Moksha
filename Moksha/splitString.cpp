#include "splitString.h"

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