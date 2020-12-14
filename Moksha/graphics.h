#pragma once
#include <windows.h>
#include <winuser.h>
#include <conio.h>
#include <WinCon.h>
#include <string>

using namespace std;

class Graphic {
private:
	int screenWidth;
	int screenHeight;
	int fps;

	// Tela
	wchar_t *screen;	// wide character array
	HANDLE console;
	DWORD bytesWritten;

	int getCoords(int x, int y);

public:
	Graphic(int screenWidth, int screenHeight, int fps);

	void draw(int x, int y, char c);
	void drawString(int x, int y, string s);
	void drawArea(int x1, int y1, int x2, int y2, char c);

	void paint(int initialX, int initialY, int length, char color);
	void paintBG(int initialX, int initialY, int length, char color);
	void update();
};