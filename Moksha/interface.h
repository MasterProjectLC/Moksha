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
	list<string> linhas;
	list<string>::iterator it;

	wchar_t *screen;	// wide character array
	HANDLE console;
	DWORD bytesWritten;

	int getCoords(int x, int y);

public:
	Interface(int screenWidth, int screenHeight, int separator, int fps);

	void interface_principal();

	void draw();

	void go_left();
	void go_right();
	void go_up();
	void go_down();

	void set_titulo(string titulo);
	void add_linha(string nova);
};