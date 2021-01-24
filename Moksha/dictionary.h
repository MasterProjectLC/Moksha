#pragma once
#include <vector>
#include <string>
#include "stringLib.h"

using namespace std;

template <typename T>
class Dictionary {
protected:
	vector<vector<string>> keys;
	vector<T> values;
public:
	Dictionary() {};

	vector<vector<string>> getKeys() { return keys; }

	T getValues(string key) {
		for (int i = 0; i < keys.size(); i++)
			for (int j = 0; j < keys[i].size(); j++)
				if (keys[i][j].compare(key) == 0 && i < values.size())
					return values[i];

		vector<string> a;
		return a;
	}


	void addPair(vector<string> key, T value) {
		keys.push_back(key);
		values.push_back(value);
	}


	void setValue(string key, T value) {
		for (int i = 0; i < keys.size(); i++)
			for (int j = 0; j < keys[i].size(); j++)
				if (keys[i][j].compare(key) == 0) {
					values[i] = value;
					break;
				}
	}


	bool hasKey(string key) {
		for (int i = 0; i < keys.size(); i++)
			for (int j = 0; j < keys[i].size(); j++)
				if (keys[i][j].compare(key) == 0)
					return true;
		return false;
	}

};