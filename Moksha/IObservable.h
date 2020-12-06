#pragma once
#include <list>
#include "IObserver.h"

using namespace std;

class IObservable {
private:
	list<IObserver*> lista;
	list<IObserver*>::iterator it;
	list<int> callcards;
	list<int>::iterator itt;

public:
	void add(IObserver* obs, int callcard);
	void remove(IObserver* obs);
	void notify();
};