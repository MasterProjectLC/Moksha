#include "interface.h"

Interface::Interface(int screenWidth, int screenHeight, int separator, int fps) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->separator = separator;
	this->pointer = 0;
	this->vPointer = 0;
	this->invPointer = 1;
	this->titulosPointer = 0;
	this->menuPointer = 0;
	this->fps = fps;
	this->clock = 0;

	titulos.push_back("Itens");
	titulos.push_back("Rumores");
	titulos.push_back("Conceitos");
	for (int k = 0; k < titulos.size(); k++) {
		int tituloCenter = spread(separator, titulos.size() + 2, k * 2);
		int tituloOffset = titulos[k].size() / 2;
		tituloPositions.push_back(tituloCenter - tituloOffset);

		itens.push_back(list<string>());
	}

	this->textTab = false;
	this->menu = false;

	graphics = Graphic(screenWidth, screenHeight, fps);
	input.add(this, 0);

	interfacePrincipal();
	interfaceInventario(invPointer, titulosPointer);
}


int Interface::spread(int size, int count, int n) {
	if (n >= count)
		return NULL;

	int interval = size / (count +1);
	return (n+1)*interval;

}

void Interface::setPointer(int n) {
	graphics.paintBG(separator + pointer + 1, screenHeight - 1, 1, 'n');
	graphics.paint(separator + pointer + 1, screenHeight - 1, 1, 'w');
	pointer = n;
	if (pointer < 0)
		pointer = 0;
	else if (pointer > linhaAtual.size())
		pointer = linhaAtual.size();
	interfaceUnderline(true);
}

void Interface::setVPointer(int n) {
	if (vPointer == 0 && n != 0)
		linhaGuardada = linhaAtual.substr(0, linhaAtual.length());

	vPointer = n;
	if (vPointer < 0)
		vPointer = 0;
	else if (vPointer > linhas.size())
		vPointer = linhas.size();

	if (vPointer == 0)
		linhaAtual = linhaGuardada.substr(0, linhaGuardada.length());
	else {
		int i = 0;
		for (it = linhas.begin(); it != linhas.end(); it++, i++) {
			if (i == vPointer-1) {
				linhaAtual = *it;
				break;
			}
		}
	}

	setPointer(linhaAtual.size());
}

void Interface::setInvPointer(int n) {
	interfaceInventario(n, titulosPointer);
}

void Interface::setTitulosPointer(int n) {
	interfaceInventario(invPointer, n);
}

void Interface::setMenuPointer(int n) {
	if (n < 0)
		menuPointer = MENU_OPTIONS.size();
	else if (n > MENU_OPTIONS.size())
		menuPointer = 0;
	else 
		menuPointer = n;
}


// DESIGN ----------------------------------------------------------------------

void Interface::interfacePrincipal() {
	interfaceTela();
	interfaceConsole();

	if (menu)
		interfaceMenu();

	// Update
	graphics.update();
}

void Interface::interfaceTela() {
	graphics.drawArea(separator + 1, 0, screenWidth, screenHeight, ' ');
	//graphics.drawArea(screenWidth, 0, screenWidth+1, screenHeight, '\n');
	graphics.drawArea(separator, 0, separator+1, screenHeight, '|');
}

void Interface::interfaceMenu() {
	int menuWidth = screenWidth / 3;
	int menuHeight = 2*screenHeight / 3;

	int y_inicio = (screenHeight - menuHeight) / 2;
	int y_fim = (screenHeight + menuHeight) / 2;
	int x_inicio = (screenWidth - menuWidth) / 2;
	int x_fim = (screenWidth + menuWidth) / 2;
	int optionToDraw = 0;

	// Draw each line
	for (int y = y_inicio; y <= y_fim && y - y_inicio <= menuProgress; y++) {
		// Draw side lines
		graphics.draw(x_inicio, y, '|');
		graphics.draw(x_fim, y, '|');

		// Draw each cell
		for (int x = 1 + x_inicio; x < x_fim; x++) {
			if (y == y_inicio || y == y_fim)
				graphics.draw(x, y, '-');
			else {
				graphics.draw(x, y, ' ');
			}
		}

		// Draw menu options
		if (optionToDraw >= MENU_OPTIONS.size())
			continue;

		if (y == y_inicio + spread(menuHeight, MENU_OPTIONS.size(), optionToDraw)) {
			int menuOptionLength = MENU_OPTIONS[optionToDraw].size();
			graphics.drawString((x_fim + x_inicio - menuOptionLength) / 2, y, MENU_OPTIONS[optionToDraw]);

			if (optionToDraw == menuPointer) {
				graphics.paint((x_fim + x_inicio - menuOptionLength) / 2, y, menuOptionLength, 'n');
				graphics.paintBG((x_fim + x_inicio - menuOptionLength) / 2, y, menuOptionLength, 'w');
			}
			else {
				graphics.paint((x_fim + x_inicio - menuOptionLength) / 2, y, menuOptionLength, 'w');
				graphics.paintBG((x_fim + x_inicio - menuOptionLength) / 2, y, menuOptionLength, 'n');
			}
			optionToDraw++;
		}
	}
	
}

void Interface::interfaceInventario(int invP, int tituloP, bool paintItem) {
	// Blank canvas
	graphics.drawArea(0, 0, separator, screenHeight, ' ');

	// Titulo
	for (int k = 0; k < titulos.size(); k++) {
		int tituloCenter = spread(separator, titulos.size()+2, k*2);

		int tituloOffset = titulos[k].size() / 2;
		graphics.drawString(tituloCenter - tituloOffset, 0, titulos[k]);
	}

	if (0 <= tituloP && tituloP < titulos.size()) {
		graphics.paint(tituloPositions[titulosPointer], 0, titulos[titulosPointer].size(), 'w');
		graphics.paintBG(tituloPositions[titulosPointer], 0, titulos[titulosPointer].size(), 'n');
		titulosPointer = tituloP;
		graphics.paint(tituloPositions[titulosPointer], 0, titulos[titulosPointer].size(), 'n');
		graphics.paintBG(tituloPositions[titulosPointer], 0, titulos[titulosPointer].size(), 'w');
	}

	// Itens
	int sobre = 1;
	for (it = itens[titulosPointer].begin(); it != itens[titulosPointer].end(); it++, sobre++) {
		string s = *it;
		sobre += s.size() / separator;
		graphics.drawString(0, sobre, s);
	}

	bool mudouItem = invPointer != invP;

	if (0 < invP) {
		graphics.paint(0, invPointer, separator, 'w');
		graphics.paintBG(0, invPointer, separator, 'n');
		if (invP <= itens[titulosPointer].size()) {
			invPointer = invP;
		}
		if (paintItem && (invP <= itens[titulosPointer].size() || mudouItem)) {
			graphics.paint(0, invPointer, separator, 'n');
			graphics.paintBG(0, invPointer, separator, 'w');
		}
	}

}

void Interface::interfaceConsole() {
	// Linha Atual
	string s = linhaAtual;
	graphics.drawString(separator + 1, screenHeight - 1, s);
	graphics.paint(separator + 1, screenHeight - 1, s.size() + 1, 'c');
	interfaceUnderline(underline);

	// Linhas
	int sobre = 1;
	for (it = linhas.begin(); it != linhas.end(); it++, sobre++) {
		s = *it;
		sobre += s.size() / (screenWidth - separator);
		graphics.drawString(separator + 1, screenHeight - sobre - 1, s);
	}
}

void Interface::interfaceUnderline(bool n) {
	underline = n;
	if (underline) {
		if (pointer < linhaAtual.size()) {
			graphics.paintBG(separator + pointer + 1, screenHeight - 1, 1, 'c');
			graphics.paint(separator + pointer + 1, screenHeight - 1, 1, 'n');
		}
		else
			graphics.draw(separator + pointer + 1, screenHeight - 1, '_');
	}
	else
		if (pointer < linhaAtual.size()) {
			graphics.paintBG(separator + pointer + 1, screenHeight - 1, 1, 'n');
			graphics.paint(separator + pointer + 1, screenHeight - 1, 1, 'c');
		}
		else
			graphics.draw(separator + pointer + 1, screenHeight - 1, ' ');
}



// EXECUÇÃO -------------------------------

void Interface::clocking() {
	input.input();

	(this->clock)++;

	// Underline pointer
	if (!textTab && this->clock % ULCOOLDOWN == 0) {
		interfaceUnderline(!underline);
		graphics.update();
		(this->clock) = 0;
	}

	// Menu progress
	if (menu && this->clock % MENUANIMATION == 0) {
		menuProgress++;
		interfaceMenu();
		graphics.update();
	}
}

// INPUT ------------------------------------------------------

void Interface::update(int id) {
	// Update
	switch (id) {
	case 0:
		inputUpdate();
		break;
	}

	// Design & Draw
	interfacePrincipal();
}

void Interface::inputUpdate() {
	if (input.getInput(input.left))
		pointerLeft();

	if (input.getInput(input.right))
		pointerRight();

	if (input.getInput(input.up))
		pointerUp();

	if (input.getInput(input.down))
		pointerDown();

	if (input.getInput(input.space))
		space();

	if (input.getInput(input.backspace))
		removeLetra(true);

	if (input.getInput(input.deleter))
		removeLetra(false);

	if (input.getInput(input.enter)) {
		if (!menu) {
			args = subirLinha();
			setNotifyID(notifyArgs);
			notify();
		}
		else {
			switch (menuPointer) {
			case 0: // Resume
				setMenu(false);
				break;
			case 2: // Quit
				HWND console = GetConsoleWindow();
				SendMessage(console, WM_CLOSE, 0, NULL);
				break;
			}
		}
	}

	if (input.getInput(input.tab))
		setTab(!getTab());

	if (input.getInput(input.escape))
		setMenu(!getMenu());

	if (input.getInput(input.typing))
		addLetra(input.getTyped());
}


void Interface::pointerLeft() {
	if (menu)
		return;

	if (textTab)
		setTitulosPointer(titulosPointer - 1);
	else
		setPointer(pointer - 1);
}

void Interface::pointerRight() {
	if (menu)
		return;

	if (textTab)
		setTitulosPointer(titulosPointer+1);
	else
		setPointer(pointer + 1);
}

void Interface::pointerUp() {
	if (menu)
		setMenuPointer(menuPointer - 1);
	else {
		if (textTab)
			setInvPointer(invPointer - 1);
		else
			setVPointer(vPointer + 1);
	}
}

void Interface::pointerDown() {
	if (menu)
		setMenuPointer(menuPointer + 1);
	else {
		if (textTab)
			setInvPointer(invPointer + 1);
		else
			setVPointer(vPointer - 1);
	}
}

void Interface::space() {
	if (menu)
		return;

	addLetra(' ');
	underline = true;
}

void Interface::setTab(boolean tab) {
	if (menu)
		return;

	this->textTab = tab;
	interfaceInventario(invPointer, titulosPointer, textTab);
}

void Interface::setMenu(boolean menu) {
	setMenuPointer(MENU_OPTIONS.size());
	if (menu == false) {
		interfaceMenu();
	}

	this->menu = menu;
	this->menuProgress = 0;
	interfaceInventario(invPointer, titulosPointer, !menu);
}

void Interface::setItens(vector<string> itens) { 
	for (int i = 0; i < itens.size(); i++) {
		if (i >= this->itens[0].size())
			this->itens[0].push_back(itens[i]);
	}

	interfaceInventario(invPointer, titulosPointer, false);
}

void Interface::addLetra(char nova) {
	if (textTab || menu)
		return;

	linhaAtual = linhaAtual.substr(0, pointer) + nova + linhaAtual.substr(pointer, linhaAtual.length());
	setPointer(pointer + 1);
}

void Interface::removeLetra(bool before) {
	if (textTab || menu)
		return;

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
	vector<string> retorno = splitString(linhaAtual, ' ');

	if (linhaAtual == "" || menu)
		return retorno;

	printLinha(linhaAtual);
	linhaGuardada = "";
	setVPointer(0);
	setPointer(0);
	return retorno;
}


void Interface::printLinha(string nova) {
	vector<string> novaCortada = splitString(nova, '\n');
	int consoleSize = screenWidth - separator - 1;

	for (int i = 0; i < novaCortada.size(); i++) {
		if (novaCortada[i].size() > consoleSize) {
			string strInteira = novaCortada[i];
			novaCortada.emplace(novaCortada.begin() + i + 1, 
								novaCortada[i].substr(min(consoleSize, novaCortada[i].find_last_of(' ', consoleSize)+1), string::npos));
			novaCortada.emplace(novaCortada.begin() + i + 1, 
								novaCortada[i].substr(0, min(consoleSize, novaCortada[i].find_last_of(' ', consoleSize))));
			novaCortada.erase(novaCortada.begin() + i);
		}
	}
	for (int i = 0; i < novaCortada.size(); i++) {
		linhas.push_front(novaCortada[i]);
		if (linhas.size() > screenHeight - 1)
			linhas.pop_back();
	}
}