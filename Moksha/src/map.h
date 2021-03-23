#pragma once
#pragma once
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include "fileManager.h"
#include "room.h"
#include "dictionary.h"
#include "IObserver.h"

using namespace std;

class Map {

private:
	class Node { // Classe utilizada pelos métodos de procura
	private:
		Room *element; // Sala representada pelo node
		int visited; // Se node foi visitado pelo método de procura ou não
		vector<int> adjacentIndex; // Index das anexas
		Node* parentIndex; // Index do node que originou este node
		bool hasParent_; // Se já foi explorado

	public:
		Node() {};

		Node(Room *element) {
			this->visited = 0;
			this->element = element;
			this->hasParent_ = false;
		};

		int getVisited() { return visited; };
		void setVisited(int v) { visited = v; };

		bool hasParent() { return hasParent_; };
		Node* getParent() { return parentIndex; }
		void setParent(Node* v) {
			if (v == this)
				hasParent_ = false;
			else {
				parentIndex = v;
				hasParent_ = true;
			}
		};

		Room* getElement() { return element; };

		void addAdjacent(int novo) { adjacentIndex.push_back(novo); };
		vector<int> getAdjacents() { return adjacentIndex; };

		bool isAdjacent(int a) { 
			for (int i = 0; i < adjacentIndex.size(); i++) {
				if (adjacentIndex[i] == a) {
					return true;
				}
			}
			return false;
		}
	};

	// Atributos
	int MAX_OBJECT_COUNT = 20;
	vector<Node> rooms;
	vector<Object> objects;
	IObserver* observer;

	void breadthSearchHelper(queue<Room*> *retorno, stack<int> &st, int salaChecada);

public:
	Map() {};
	Map(vector<Room*> receivedRooms, IObserver *observer);

	void loadRoom(Room *room);
	Object* getObject(int id);

	queue<Room*> optimalPath(Room *_origin, Room *_destination);
	queue<Room*> breadthSearch(Room *_origin);
	bool hasRoom(string codename);
	bool hasRoomByName(string name);
	Room* getRoom(int index);
	Room* getRoom(string codename);
	Room* getRoomByName(string name);

	void clearVisited();
};