#pragma once
#include <windows.h>
#include <winuser.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <list>
#include <iterator>

using namespace std;

class Interface {
	// Atributos
private:
	int screenWidth;
	int screenHeight;
	int separator;
	int selector;
	int fps;

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

	void draw();

	void goLeft();
	void goRight();
	void goUp();
	void goDown();

	void setTitulo(string titulo);
	void addLetra(char nova);
	void subirLinha();
};