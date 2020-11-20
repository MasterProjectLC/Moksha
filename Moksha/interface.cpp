#include "interface.h"

Interface::Interface(int screenWidth, int screenHeight, int separator, int fps) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->separator = separator;
	this->selector = separator + 1;
	this->fps = fps;

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
			else
				screen[position] = '|';

			if (selector == position)
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
	for (int i = 0; i < s.size(); i++)
		screen[getCoords(separator + i + 1, screenHeight - 1)] = s[i];

	// Linhas
	int sobre = 1;
	for (it = linhas.begin(); it != linhas.end(); it++, sobre++) {
		s = *it;
		sobre += s.size() / (screenWidth - separator);
		for (int i = 0; i < s.size(); i++)
			screen[getCoords(separator + i + 1, screenHeight - sobre - 1)] = s[i];
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