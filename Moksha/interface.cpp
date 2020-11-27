#include "interface.h"

Interface::Interface(int screenWidth, int screenHeight, int separator, int fps) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->separator = separator;
	this->pointer = 0;
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

void Interface::setPointer(int n) {
	pointer = n;
	if (pointer < 0)
		pointer = 0;
	else if (pointer > linhaAtual.size())
		pointer = linhaAtual.size();
}


// DESIGN ----------------------------------------------------------------------

void Interface::interfacePrincipal() {
	// Tela principal
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
	interfaceUnderline(underline);
	paint(separator+1, screenHeight - 1, s.size()+1, 'c');

	// Linhas
	int sobre = 1;
	for (it = linhas.begin(); it != linhas.end(); it++, sobre++) {
		s = *it;
		sobre += s.size() / (screenWidth - separator);
		for (int i = 0; i < s.size(); i++)
			screen[getCoords(separator + i + 1, screenHeight - sobre - 1)] = s[i];
	}
}


void Interface::interfaceUnderline(bool n) {
	this->clock = 0;
	underline = n;
	if (underline) {
		if (pointer < linhaAtual.size())
			paintBG(separator + pointer + 1, screenHeight - 1, 1, 'w');
		else
			screen[getCoords(separator + pointer + 1, screenHeight - 1)] = '_';
	}
	else
		if (pointer < linhaAtual.size())
			paintBG(separator + pointer + 1, screenHeight - 1, 1, 'b');
		else
			screen[getCoords(separator + pointer + 1, screenHeight - 1)] = ' ';
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

void Interface::paintBG(int initialX, int initialY, int length, char color) {
	COORD coord;
	coord.X = initialX; coord.Y = initialY;
	DWORD actual;

	switch (color) {
	case 'b':
		FillConsoleOutputAttribute(console, BACKGROUND_BLUE, length, coord, &actual);
		break;
	case 'r':
		FillConsoleOutputAttribute(console, BACKGROUND_RED, length, coord, &actual);
		break;
	case 'g':
		FillConsoleOutputAttribute(console, BACKGROUND_GREEN, length, coord, &actual);
		break;
	case 'p':
		FillConsoleOutputAttribute(console, BACKGROUND_BLUE | BACKGROUND_RED, length, coord, &actual);
		break;
	case 'y':
		FillConsoleOutputAttribute(console, BACKGROUND_RED | BACKGROUND_GREEN, length, coord, &actual);
		break;
	case 'c':
		FillConsoleOutputAttribute(console, BACKGROUND_GREEN | BACKGROUND_BLUE, length, coord, &actual);
		break;
	case 'w':
		FillConsoleOutputAttribute(console, BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED, length, coord, &actual);
		break;
	case 'n':
		FillConsoleOutputAttribute(console, !BACKGROUND_GREEN | !BACKGROUND_BLUE | !BACKGROUND_RED, length, coord, &actual);
		break;
	}
}


// EXECUÇÃO -------------------------------

void Interface::clocking() {
	(this->clock)++;
	if (this->clock > ULCOOLDOWN) {
		interfaceUnderline(!underline);
		draw();
	}
}

void Interface::draw() {
	WriteConsoleOutputCharacterW(console, screen, screenWidth*screenHeight, { 0, 0 }, &bytesWritten);
	Sleep(1000 / fps);
}


// INPUT ------------------------------------------------------

void Interface::goLeft() {
	setPointer(pointer - 1);
}

void Interface::goRight() {
	setPointer(pointer + 1);
}

void Interface::goUp() {

}

void Interface::goDown() {

}

void Interface::space() {
	addLetra(' ');
	underline = true;
}

void Interface::setTitulo(string titulo) {
	this->titulo = titulo;
}

void Interface::addLetra(char nova) {
	linhaAtual = linhaAtual.substr(0, pointer) + nova + linhaAtual.substr(pointer, linhaAtual.length());
	setPointer(pointer + 1);
}

void Interface::removeLetra(bool before) {
	if (before && pointer > 0) {
		linhaAtual = linhaAtual.substr(0, pointer-1) + linhaAtual.substr(pointer, linhaAtual.length());
		setPointer(pointer - 1);
	}
	else if (pointer < linhaAtual.length()) {
		linhaAtual = linhaAtual.substr(0, pointer) + linhaAtual.substr(pointer + 1, linhaAtual.length());
	}

	underline = true;
}


vector<string> Interface::subirLinha() {
	vector<string> retorno;

	if (linhaAtual == "")
		return retorno;

	printLinha(linhaAtual);

	// Credits - techiedelight
	size_t start;
	size_t end = 0;

	while ((start = linhaAtual.find_first_not_of(' ', end)) != std::string::npos)
	{
		end = linhaAtual.find(' ', start);
		retorno.push_back(linhaAtual.substr(start, end - start));
	}

	linhaAtual = "";
	return retorno;
}


void Interface::printLinha(string nova) {
	linhas.push_front(nova);
	setPointer(0);
	if (linhas.size() > screenHeight - 1)
		linhas.pop_back();
}