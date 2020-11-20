#pragma once
#include <windows.h>

class IObserver {
public:
	virtual void update()
	{
		OutputDebugStringW(L"My output string.");
	};
};