#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "stringLib.h"

using namespace std;

class FileDict {
private:
	vector<vector<string>> keys;
	vector<vector<string>> values;
public:
	FileDict(string fileText);

	vector<vector<string>> getKeys() { return keys; };
	string getValue(string key);
	vector<string> getValues(string key);

};