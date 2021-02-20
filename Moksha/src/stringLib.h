#pragma once
#include <iostream>
#include <string>
#include <sstream> 
#include <vector>
#include <set>
#include <iterator>

using namespace std;

const vector<string> splitString(string str, char delim = ' ');
const set<string> splitStringSet(string str, char delim = ' ');
const string concatStrings(vector<string> args, int inicio);
const vector<string> editVector(string prefix, vector<string> vetor, string suffix);
const vector<string*> editVector(string prefix, vector<string*> vetor, string suffix);
const bool stringEqual(string str1, string str2);