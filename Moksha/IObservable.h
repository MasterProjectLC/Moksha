#pragma once
#include <list>
#include "IObserver.h"

using namespace std;

class IObservable {
private:
	list<IObserver*> lista;
	list<IObserver*>::iterator it;


public:
	void add(IObserver* obs);
	void remove(IObserver* obs);
	void notify();
};