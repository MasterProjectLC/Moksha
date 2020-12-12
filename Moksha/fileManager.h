#pragma once
#include <Windows.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include "splitString.h"

using namespace std;

class FileManager {
private:

public:
	FileManager();

	vector<string> getFileList(string folder);
	string readFromFile(string path);
};