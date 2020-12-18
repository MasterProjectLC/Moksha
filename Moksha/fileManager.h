#pragma once
#include <Windows.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include "stringLib.h"
#include "fileDict.h"

using namespace std;

class FileManager {
private:

public:
	FileManager();

	vector<string> getFileList(string folder);
	FileDict readFromFile(string path);
	
};