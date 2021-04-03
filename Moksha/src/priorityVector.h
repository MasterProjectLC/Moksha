#pragma once
#include <vector>

using namespace std;

template <typename T>
class PriorityVector {
private:
	vector<T> v;
	bool(*comparison)(T, T);

public:
	PriorityVector() {}
	PriorityVector(vector<T> vec, bool(*compPointer)(T, T)) { 
		v = vec; 
		comparison = compPointer; 
		make_heap(v.begin(), v.end(), comparison); 
	}

	vector<T> getVector() { return v; }
	typename vector<T>::iterator highest() { return v.end()-1; }
	typename vector<T>::iterator lowest() { return v.begin(); }
	void ascend(typename vector<T>::iterator& it) { if (it != v.end() - 1) { it++; }; }
	void descend(typename vector<T>::iterator& it) { if (it != v.begin()) { it--; }; }
	bool empty() { return v.empty(); }
	void push(T element) { v.push_back(element); push_heap(v.begin(), v.end(), comparison); }
	T pop() { T retorno = v.back(); v.pop_back(); pop_heap(v.begin(), v.end(), comparison); return retorno; }
	void sort() { make_heap(v.begin(), v.end(), comparison); sort_heap(v.begin(), v.end(), comparison); }
	void clear() { v.clear(); }
};