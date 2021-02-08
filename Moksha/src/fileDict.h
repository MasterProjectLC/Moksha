#pragma once
#include "dictionary.h"
#include "stringLib.h"

using namespace std;

class FileDict: public Dictionary<vector<string>> {
public:
	FileDict(string fileText);
};