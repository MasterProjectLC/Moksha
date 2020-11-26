#include "interface.h"

Interface::Interface(int screenWidth, int screenHeight, int separator, int fps) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->separator = separator;
	this->selector = separator + 1;
	this->fps = fps;
	this->clock = 0;

	titulo = ""; 

	screen = new wchar_t[screenWidth*screenHeight];
	console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(console);
	bytesWritten = 0;
}


int Interface::getCoords(int x, int y) {
	return y * screenWidth + x;
}


void Interface::interfacePrincipal() {
	for (int i = 0; i < screenHeight; i++) {
		for (int j = 0; j < screenWidth; j++) {
			int position = getCoords(j, i);

			if (j < separator)
				screen[position] = '#';
			else if (j > separator)
				screen[position] = ' ';
			else if (j == screenWidth-1)
				screen[position] = '\n';
			else
				screen[position] = '|';

			if (selector == position && j != screenWidth - 1)
				screen[position] = 'O';
		}
	}
	screen[screenWidth * screenHeight - 1] = '\0';

	// Titulo
	int tituloCenter = separator / 2;
	int tituloOffset = titulo.size() / 2;
	for (int i = tituloCenter - tituloOffset, j = 0; j < titulo.size(); i++, j++) {
		screen[i] = titulo[j];
	}

	// Linha Atual
	string s = linhaAtual;
	for (int i = 0; i < s.size(); i++) {
		screen[getCoords(separator + i + 1, screenHeight - 1)] = s[i];
	}
	paint(separator+1, screenHeight - 1, s.size(), 'c');

	// Linhas
	int sobre = 1;
	for (it = linhas.begin(); it != linhas.end(); it++, sobre++) {
		s = *it;
		sobre += s.size() / (screenWidth - separator);
		for (int i = 0; i < s.size(); i++)
			screen[getCoords(separator + i + 1, screenHeight - sobre - 1)] = s[i];
	}
}


void Interface::clocking() {
	(this->clock)++;
	if (this->clock > ULCOOLDOWN) {
		this->clock = 0;
		underline = !(underline);
		if (underline)
			screen[getCoords(separator + linhaAtual.size() + 1, screenHeight - 1)] = '_';
		else
			screen[getCoords(separator + linhaAtual.size() + 1, screenHeight - 1)] = ' ';

		draw();
	}
}


void Interface::paint(int initialX, int initialY, int length, char color) {
	COORD coord;
	coord.X = initialX; coord.Y = initialY;
	DWORD actual;

	switch (color) {
	case 'b':
		FillConsoleOutputAttribute(console, FOREGROUND_BLUE, length, coord, &actual);
		break;
	case 'r':
		FillConsoleOutputAttribute(console, FOREGROUND_RED, length, coord, &actual);
		break;
	case 'g':
		FillConsoleOutputAttribute(console, FOREGROUND_GREEN, length, coord, &actual);
		break;
	case 'p':
		FillConsoleOutputAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED, length, coord, &actual);
		break;
	case 'y':
		FillConsoleOutputAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN, length, coord, &actual);
		break;
	case 'c':
		FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_BLUE, length, coord, &actual);
		break;
	}
}


void Interface::draw() {
	WriteConsoleOutputCharacterW(console, screen, screenWidth*screenHeight, { 0, 0 }, &bytesWritten);
	Sleep(1000 / fps);
}


void Interface::goLeft() {
	selector--;
	selector += (selector % screenWidth <= separator) * (screenWidth - separator);
}

void Interface::goRight() {
	selector++;
	selector += (selector % screenWidth <= separator) * (separator + 1 - screenWidth);
}

void Interface::goUp() {
	selector = selector - screenWidth;
	selector += (selector < 0) * screenWidth * screenHeight;
}

void Interface::goDown() {
	selector = selector + screenWidth;
	selector -= (selector > screenWidth * screenHeight) * screenWidth * screenHeight;
}

void Interface::space() {
	linhaAtual += ' ';
}

void Interface::backspace() {
	linhaAtual = linhaAtual.substr(0, linhaAtual.length() - 1);
}

void Interface::setTitulo(string titulo) {
	this->titulo = titulo;
}

void Interface::addLetra(char nova) {
	linhaAtual += nova;
}

void Interface::subirLinha() {
	if (linhaAtual == "")
		return;

	linhas.push_front(linhaAtual);
	linhaAtual = "";
	if (linhas.size() > screenHeight-1)
		linhas.pop_back();
}