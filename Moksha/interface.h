#pragma once
#include <iostream>
#include <string>
#include <numeric>
#include <sstream> 
#include <vector>
#include <list>
#include <iterator>
#include "graphics.h"
#include "IObserver.h"
#include "input.h"
#include "splitString.h"

using namespace std;

class Interface : public IObserver, public IObservable {
	// Atributos
private:
	int const ULCOOLDOWN = 10000;
	int const MENUANIMATION = 100;
	int const CLOCKCD = ULCOOLDOWN;

	vector<string> const MENU_OPTIONS = { "CONTINUAR", "OPCOES", "SAIR" };

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
	string linhaAtual = "";
	string linhaGuardada = "";
	list<string> linhas;
	list<string>::iterator it;

	void addLetra(char nova);
	void removeLetra(bool before);
	vector<string> subirLinha();

	// Inventario
	string titulo;
	list<string> itens;

	// Pointers
	int pointer;
	int vPointer;
	int invPointer;
	int menuPointer;

	void setPointer(int n);
	void setVPointer(int n);
	void setInvPointer(int n);
	void setMenuPointer(int n);

	// Visual Interface
	void interfaceTela();
	void interfaceMenu();
	void interfaceConsole();
	void interfaceInventario(int n);
	void interfaceUnderline(bool n);

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

	void interfacePrincipal();
	void clocking();
	void printLinha(string nova);

	enum {notifyArgs};

	// Getters/Setters
	vector<string> getArgs() { return args; }
	boolean getTab() { return textTab; };
	boolean getMenu() { return menu; };

	void setTab(boolean tab);
	void setMenu(boolean menu);
	void setTitulo(string titulo);
	void setItens(vector<string> itens);

};