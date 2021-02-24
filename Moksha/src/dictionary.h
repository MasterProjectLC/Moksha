#pragma once
#include <map>
#include <set>
#include <vector>
#include <string>
#include "stringLib.h"
#include <stdexcept>

using namespace std;

template <typename T>
class Dictionary {
protected:
	map<set<string>, T> dictionary;

public:
	void addPair(set<string> key, T value) {
		if (hasKey(*key.begin()))
			dictionary.erase(key);

		dictionary.insert(pair<set<string>, T>(key, value));
	}

	virtual string getValue(string key) {
		return "";
	}


	T getValues(string key) {
		for (typename map<set<string>, T>::iterator it = dictionary.begin(); it != dictionary.end(); it++)
			if (it->first.count(key))
				return it->second;

		throw invalid_argument("No value with that key");
	}

	bool hasKey(string key) {
		for (typename map<set<string>, T>::iterator it = dictionary.begin(); it != dictionary.end(); it++)
			if (it->first.count(key))
				return true;
		return false;
	}
};

string Dictionary<vector<string>>::getValue(string key) {
	return *getValues(key).begin();
};
