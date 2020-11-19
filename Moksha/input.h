#pragma once
#include <conio.h>
#include <ctype.h>

using namespace std;

class Input
{
public:
	template<typename Function>
	bool input(Function left, Function right, Function up, Function down);
};