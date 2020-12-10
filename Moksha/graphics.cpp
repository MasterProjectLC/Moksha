#include "graphics.h"

Graphic::Graphic(int screenWidth, int screenHeight, int fps) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->fps = fps;

	screen = new wchar_t[screenWidth*screenHeight];
	console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(console);
	bytesWritten = 0;
}

int Graphic::getCoords(int x, int y) {
	return y * screenWidth + x;
}

void Graphic::draw(int x, int y, char c) {
	screen[getCoords(x, y)] = c;
}

void Graphic::drawString(int x, int y, string s) {
	for (int i = 0; i < s.size(); i++)
		draw(i+x, y, s[i]);
}

void Graphic::paint(int initialX, int initialY, int length, char color) {
	COORD coord;
	coord.X = initialX; coord.Y = initialY;
	DWORD actual;
	WORD current[100];

	ReadConsoleOutputAttribute(console, current, length, coord, &actual);

	for (int i = 0; i < length; i++) {
		current[i] = current[i] & (BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
		switch (color) {
		case 'b':
			FillConsoleOutputAttribute(console, current[i] | FOREGROUND_BLUE, 1, coord, &actual);
			break;
		case 'r':
			FillConsoleOutputAttribute(console, current[i] | FOREGROUND_RED, 1, coord, &actual);
			break;
		case 'g':
			FillConsoleOutputAttribute(console, current[i] | FOREGROUND_GREEN, 1, coord, &actual);
			break;
		case 'p':
			FillConsoleOutputAttribute(console, current[i] | FOREGROUND_BLUE | FOREGROUND_RED, 1, coord, &actual);
			break;
		case 'y':
			FillConsoleOutputAttribute(console, current[i] | FOREGROUND_RED | FOREGROUND_GREEN, 1, coord, &actual);
			break;
		case 'c':
			FillConsoleOutputAttribute(console, current[i] | FOREGROUND_BLUE | FOREGROUND_GREEN, 1, coord, &actual);
			break;
		case 'w':
			FillConsoleOutputAttribute(console, current[i] | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN, 1, coord, &actual);
			break;
		case 'n':
			FillConsoleOutputAttribute(console, current[i], 1, coord, &actual);
			break;
		}

		if (coord.X < screenWidth) { coord.X++; }
		else { coord.X = 0; coord.Y++; }
	}
}

void Graphic::paintBG(int initialX, int initialY, int length, char color) {
	COORD coord;
	coord.X = initialX; coord.Y = initialY;
	DWORD actual;
	WORD current[100];

	ReadConsoleOutputAttribute(console, current, length, coord, &actual);

	for (int i = 0; i < length; i++) {
		current[i] = current[i] & (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
		switch (color) {
		case 'b':
			FillConsoleOutputAttribute(console, current[i] | BACKGROUND_BLUE, 1, coord, &actual);
			break;
		case 'r':
			FillConsoleOutputAttribute(console, current[i] | BACKGROUND_RED, 1, coord, &actual);
			break;
		case 'g':
			FillConsoleOutputAttribute(console, current[i] | BACKGROUND_GREEN, 1, coord, &actual);
			break;
		case 'p':
			FillConsoleOutputAttribute(console, current[i] | BACKGROUND_BLUE | BACKGROUND_RED, 1, coord, &actual);
			break;
		case 'y':
			FillConsoleOutputAttribute(console, current[i] | BACKGROUND_RED | BACKGROUND_GREEN, 1, coord, &actual);
			break;
		case 'c':
			FillConsoleOutputAttribute(console, current[i] | BACKGROUND_BLUE | BACKGROUND_GREEN, 1, coord, &actual);
			break;
		case 'w':
			FillConsoleOutputAttribute(console, current[i] | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN, 1, coord, &actual);
			break;
		case 'n':
			FillConsoleOutputAttribute(console, current[i], 1, coord, &actual);
			break;
		}

		if (coord.X < screenWidth) { coord.X++; }
		else { coord.X = 0; coord.Y++; }
	}
}


void Graphic::update() {
	screen[screenWidth*screenHeight - 1] = '\0';
	WriteConsoleOutputCharacterW(console, screen, screenWidth*screenHeight, { 0, 0 }, &bytesWritten);
	Sleep(1000 / fps);
}