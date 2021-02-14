#pragma once
#include <iostream>
#include <string>
#include <sstream> 
#include <vector>
#include <set>
#include <iterator>

using namespace std;

vector<string> splitString(string str, char delim = ' ');
set<string> splitStringSet(string str, char delim = ' ');
string concatStrings(vector<string> args, int inicio);
bool stringEqual(string str1, string str2);