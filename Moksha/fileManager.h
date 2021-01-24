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
	FileManager() {}

	static vector<string> getFileList(string folder)
	{
		vector<string> names;
		string search_path = folder + "/*.*";
		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					names.push_back(folder + "/" + fd.cFileName);
				}
			} while (::FindNextFile(hFind, &fd));
			::FindClose(hFind);
		}
		return names;
	}

	static FileDict readFromFile(string path) {
		string line;
		ifstream myfile(path);
		string retorno = "";

		if (myfile.is_open()) {
			while (getline(myfile, line))
				retorno.append(line + '\n');
			myfile.close();
		}

		return FileDict(retorno);
	}
	
};