#pragma once
#include <windows.h>
#include <winuser.h>
#include <conio.h>
#include <WinCon.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <list>
#include <iterator>
#include "IObservable.h"

using namespace std;

class Interface : public IObservable {
	// Atributos
private:
	int screenWidth;
	int screenHeight;
	int separator;
	int selector;
	int fps;

	int clock = 0;
	boolean underline = false;
	int const ULCOOLDOWN = 6000;

	string titulo;
	string linhaAtual = "";
	list<string> linhas;
	list<string>::iterator it;

	wchar_t *screen;	// wide character array
	HANDLE console;
	DWORD bytesWritten;

	int getCoords(int x, int y);

public:
	Interface(int screenWidth, int screenHeight, int separator, int fps);

	void interfacePrincipal();
	void clocking();

	void draw();
	void paint(int initialX, int initialY, int length, char color);

	void goLeft();
	void goRight();
	void goUp();
	void goDown();
	void space();
	void backspace();

	void setTitulo(string titulo);
	void addLetra(char nova);
	void subirLinha();
};