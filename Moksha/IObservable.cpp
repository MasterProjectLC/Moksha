#include "IObservable.h"

void IObservable::add(IObserver* obs) {
	lista.push_front(obs);
};

void IObservable::remove(IObserver* obs) {
	
	for (it = lista.begin(); *it != obs; it++) {
	}
	lista.erase(it);
};

void IObservable::notify() {
	for (it = lista.begin(); it != lista.end(); it++) {
		(*it)->update();
	}

};
