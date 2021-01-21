#pragma once
#include "dictionary.h"

using namespace std;

class FileDict: public Dictionary {
public:
	FileDict(string fileText);
};