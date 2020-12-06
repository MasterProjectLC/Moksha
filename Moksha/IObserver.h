#pragma once
#include <windows.h>

class IObserver {
public:
	virtual void update(int id)
	{
		OutputDebugStringW(L"My output string.");
	};
};