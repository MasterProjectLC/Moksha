#pragma once
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <json/json.h>

using namespace std;
using namespace Json;

class FileManager {
private:
	void jsonParse(string n);
	void readFromFile();

public:
	FileManager();
};