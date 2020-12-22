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
	int notifyID = 0;

public:
	void add(IObserver* obs, int callcard);
	void remove(IObserver* obs);
	void notify();
	void notify(int id);

	int getNotifyID() { return notifyID; }
	void setNotifyID(int n) { notifyID = n; }
};