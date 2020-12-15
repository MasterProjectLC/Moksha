#pragma once
#include <iostream>
#include <string>
#include <sstream> 
#include <vector>
#include <iterator>

using namespace std;

vector<string> splitString(string str, char delim = ' ');
string concatStrings(vector<string> args, int inicio);