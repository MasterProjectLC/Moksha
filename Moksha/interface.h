#pragma once
#include <windows.h>
#include <winuser.h>
#include <conio.h>
#include <WinCon.h>
#include <stdlib.h>
#include <iostream>
#include <string>
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
	int fps;

	int clock = 0;
	boolean underline = false;
	int const ULCOOLDOWN = 10000;

	string titulo;
	string linhaAtual = "";
	list<string> linhas;
	list<string>::iterator it;

	wchar_t *screen;	// wide character array
	HANDLE console;
	DWORD bytesWritten;

	int getCoords(int x, int y);
	void setPointer(int n);

public:
	Interface(int screenWidth, int screenHeight, int separator, int fps);

	void interfacePrincipal();
	void interfaceMenu();
	void interfaceUnderline(bool n);
	void paint(int initialX, int initialY, int length, char color);
	void paintBG(int initialX, int initialY, int length, char color);

	void clocking();
	void draw();

	void goLeft();
	void goRight();
	void goUp();
	void goDown();
	void space();

	void setTitulo(string titulo);
	void addLetra(char nova);
	void removeLetra(bool before);
	vector<string> subirLinha();
	void printLinha(string nova);
};