#pragma once
#include <windows.h>
#include <winuser.h>
#include <conio.h>
#include <WinCon.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream> 
#include <vector>
#include <list>
#include <iterator>

using namespace std;

class Interface {
	// Atributos
private:
	int screenWidth;
	int screenHeight;
	int separator;
	int pointer;
	int vPointer;
	int fps;
	boolean textTab;

	int clock = 0;
	boolean underline = false;
	int const ULCOOLDOWN = 10000;

	// Console
	string linhaAtual = "";
	string linhaGuardada = "";
	list<string> linhas;
	list<string>::iterator it;

	// Inventario
	string titulo;
	list<string> itens;

	// Tela
	wchar_t *screen;	// wide character array
	HANDLE console;
	DWORD bytesWritten;

	int getCoords(int x, int y);
	void setPointer(int n);
	void setVPointer(int n);

public:
	Interface(int screenWidth, int screenHeight, int separator, int fps);

	void interfacePrincipal();
	void interfaceMenu();
	void interfaceUnderline(bool n);
	void paint(int initialX, int initialY, int length, char color);
	void paintBG(int initialX, int initialY, int length, char color);

	void clocking();
	void draw();

	void pointerLeft();
	void pointerRight();
	void pointerUp();
	void pointerDown();
	void space();

	void setTitulo(string titulo);
	void addLetra(char nova);
	void removeLetra(bool before);
	vector<string> subirLinha();
	void printLinha(string nova);

	boolean getTab() { return textTab; };
	void setTab(boolean tab);
	void setItens(vector<string> itens);
};