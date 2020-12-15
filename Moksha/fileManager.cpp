#include "fileManager.h"

FileManager::FileManager()
{
}


vector<string> FileManager::getFileList(string folder)
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


string FileManager::readFromFile(string path) {
	string line;
	ifstream myfile(path);
	string retorno = "";

	if (myfile.is_open()) {
		while (getline(myfile, line))
			retorno.append(line + '\n');
		myfile.close();
	}

	return retorno;
}