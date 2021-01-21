#include "IObservable.h"

void IObservable::add(IObserver* obs, int callcard) {
	lista.push_front(obs);
	callcards.push_front(callcard);
};

void IObservable::remove(IObserver* obs) {
	int i = 0;
	for (it = lista.begin(); *it != obs; it++, i++) {
	}
	lista.erase(it);
	callcards.remove(i);
};

void IObservable::notify() {
	for (it = lista.begin(), itt = callcards.begin(); it != lista.end() && itt != callcards.end(); it++, itt++) {
		(*it)->update(*itt);
	}
};

void IObservable::notify(int id) {
	setNotifyID(id);
	notify();
}
