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

void Interface::interface_principal() {
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

	// Linhas
	int sobre;
	for (sobre = 0, it = linhas.begin(); it != linhas.end(); it++, sobre++) {
		string s = *it;
		sobre += s.size() / (screenWidth - separator);
		for (int i = 0; i < s.size(); i++) {
			screen[getCoords(separator + i + 1, screenHeight - sobre - 1)] = s[i];
		}
	}
}

void Interface::draw() {
	WriteConsoleOutputCharacterW(console, screen, screenWidth*screenHeight, { 0, 0 }, &bytesWritten);
	Sleep(1000 / fps);
}


void Interface::go_left() {
	selector--;
	selector += (selector % screenWidth <= separator) * (screenWidth - separator);
}

void Interface::go_right() {
	selector++;
	selector += (selector % screenWidth <= separator) * (separator + 1 - screenWidth);
}

void Interface::go_up() {
	selector = selector - screenWidth;
	selector += (selector < 0) * screenWidth * screenHeight;
}

void Interface::go_down() {
	selector = selector + screenWidth;
	selector -= (selector > screenWidth * screenHeight) * screenWidth * screenHeight;
}

void Interface::set_titulo(string titulo) {
	this->titulo = titulo;
}

void Interface::add_linha(string nova) {
	set_titulo(nova);
	linhas.push_front(nova);
	if (linhas.size() > screenHeight)
		linhas.pop_back();
}