#pragma once
#include <iostream>
#include <string>
#include <numeric>
#include <sstream> 
#include <vector>
#include <list>
#include <iterator>
#include <conio.h>
#include <ctype.h>
# pragma comment(lib, "secur32.lib")
# pragma comment(lib, "winmm.lib")
# pragma comment(lib, "dmoguids.lib")
# pragma comment(lib, "wmcodecdspuuid.lib")
# pragma comment(lib, "msdmo.lib")
# pragma comment(lib, "Strmiids.lib")
#include <timeapi.h>
#include "graphics.h"
#include "IObserver.h"
#include "input.h"
#include "stringLib.h"

using namespace std;

class Interface : public IObserver, public IObservable {
	// Atributos
private:
	int const ULCOOLDOWN = 10000;
	int const MENUANIMATION = 100;
	int const CLOCKCD = ULCOOLDOWN;
	int time = 0;
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	vector<string> const MENU_OPTIONS = { "CONTINUE", "OPTIONS", "QUIT" };

	Graphic graphics = Graphic(0, 0, 0);
	Input input = Input();

	int spread(int size, int n, int i);

	// Screen & Separator
	int screenWidth;
	int screenHeight;
	int separator;
	int fps;

	// Options
	boolean textTab;
	boolean menu;
	int menuProgress = 0;

	int clock = 0;
	boolean underline = false;

	// Console
	string currentLine = "";
	string savedLine = "";
	list<string> lines;
	list<string> sentLines;
	list<string>::iterator it;

	void addLetter(char nova);
	void removeLetter(bool before);
	vector<string> riseLine();

	// Inventario
	vector<string> titles;
	vector<int> titlePositions;
	vector<list<string>> inventory;

	// Pointers
	int pointer;
	int vPointer;
	int invPointer;
	int titlesPointer;
	int menuPointer;

	void setPointer(int n);
	void setVPointer(int n);
	void setInvPointer(int n);
	void setTitulosPointer(int n);
	void setMenuPointer(int n);

	// Visual Interface
	void screenInterface();
	void menuInterface();
	void consoleInterface();
	void inventoryInterface(int invP, int titleP, bool paintItem = true);
	void underlineInterface(bool n);

	// Input
	void update(int id) override;
	void inputUpdate();
	void pointerLeft();
	void pointerRight();
	void pointerUp();
	void pointerDown();
	void space();

	vector<string> args;

public:
	Interface(int screenWidth, int screenHeight, int separator, int fps);

	void mainInterface();
	void gameLoop();
	void clocking();
	void printLine(string nova);

	enum {notifyArgs, item, rumor, concept};

	// Getters/Setters
	vector<string> getArgs() { return args; }
	boolean getTab() { return textTab; };
	boolean getMenu() { return menu; };

	void setTab(boolean tab);
	void setMenu(boolean menu);
	void setItems(vector<string> items, int type);
	void setConcepts(vector<string> concepts) { setItems(concepts, 2); }

};