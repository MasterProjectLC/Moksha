#include "interface.h"

Interface::Interface(int screenWidth, int screenHeight, int separator, int fps) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->separator = separator;
	this->pointer = 0;
	this->vPointer = 0;
	this->invPointer = 1;
	this->titlesPointer = 0;
	this->menuPointer = 0;
	this->fps = fps;
	this->clock = 0;

	titles.push_back("Items");
	titles.push_back("Rumors");
	titles.push_back("Concepts");
	for (int k = 0; k < titles.size(); k++) {
		int titleCenter = spread(separator, titles.size() + 2, k * 2);
		int titleOffset = titles[k].size() / 2;
		titlePositions.push_back(titleCenter - titleOffset);

		inventory.push_back(list<Concept*>());
	}

	this->textTab = false;
	this->menu = false;

	graphics = Graphic(screenWidth, screenHeight, fps);
	input.add(this, 0);

	mainInterface();
	inventoryInterface(invPointer, titlesPointer);
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
	else if (pointer > currentLine.size())
		pointer = currentLine.size();
	underlineInterface(true);
}

void Interface::setVPointer(int n) {
	if (vPointer == 0 && n != 0)
		savedLine = currentLine.substr(0, currentLine.length());

	vPointer = n;
	if (vPointer < 0)
		vPointer = 0;
	else if (vPointer > sentLines.size())
		vPointer = sentLines.size();

	if (vPointer == 0)
		currentLine = savedLine.substr(0, savedLine.length());
	else {
		int i = 0;
		for (it = sentLines.begin(); it != sentLines.end(); it++, i++) {
			if (i == vPointer-1) {
				currentLine = *it;
				break;
			}
		}
	}

	setPointer(currentLine.size());
}

void Interface::setInvPointer(int n) {
	inventoryInterface(n, titlesPointer);
}

void Interface::setTitulosPointer(int n) {
	inventoryInterface(invPointer, n);
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

void Interface::mainInterface() {
	screenInterface();
	consoleInterface();

	if (menu)
		menuInterface();

	// Update
	graphics.update();
}

void Interface::screenInterface() {
	graphics.drawArea(separator + 1, 0, screenWidth, screenHeight, ' ');
	//graphics.drawArea(screenWidth, 0, screenWidth+1, screenHeight, '\n');
	graphics.drawArea(separator, 0, separator+1, screenHeight, '|');
}

void Interface::menuInterface() {
	int menuWidth = screenWidth / 3;
	int menuHeight = 2*screenHeight / 3;

	int y_begin = (screenHeight - menuHeight) / 2;
	int y_end = (screenHeight + menuHeight) / 2;
	int x_begin = (screenWidth - menuWidth) / 2;
	int x_end = (screenWidth + menuWidth) / 2;
	int optionToDraw = 0;

	// Draw each line
	for (int y = y_begin; y <= y_end && y - y_begin <= menuProgress; y++) {
		// Draw side lines
		graphics.draw(x_begin, y, '|');
		graphics.draw(x_end, y, '|');

		// Draw each cell
		for (int x = 1 + x_begin; x < x_end; x++) {
			if (y == y_begin || y == y_end)
				graphics.draw(x, y, '-');
			else {
				graphics.draw(x, y, ' ');
			}
		}

		// Draw menu options
		if (optionToDraw >= MENU_OPTIONS.size())
			continue;

		if (y == y_begin + spread(menuHeight, MENU_OPTIONS.size(), optionToDraw)) {
			int menuOptionLength = MENU_OPTIONS[optionToDraw].size();
			graphics.drawString((x_end + x_begin - menuOptionLength) / 2, y, MENU_OPTIONS[optionToDraw]);

			if (optionToDraw == menuPointer) {
				graphics.paint((x_end + x_begin - menuOptionLength) / 2, y, menuOptionLength, 'n');
				graphics.paintBG((x_end + x_begin - menuOptionLength) / 2, y, menuOptionLength, 'w');
			}
			else {
				graphics.paint((x_end + x_begin - menuOptionLength) / 2, y, menuOptionLength, 'w');
				graphics.paintBG((x_end + x_begin - menuOptionLength) / 2, y, menuOptionLength, 'n');
			}
			optionToDraw++;
		}
	}
	
}

void Interface::inventoryInterface(int invP, int titleP, bool paintItem) {
	// Blank canvas
	graphics.drawArea(0, 0, separator, screenHeight, ' ');

	// Titulo
	for (int k = 0; k < titles.size(); k++) {
		int tituloCenter = spread(separator, titles.size()+2, k*2);

		int tituloOffset = titles[k].size() / 2;
		graphics.drawString(tituloCenter - tituloOffset, 0, titles[k]);
	}

	if (0 <= titleP && titleP < titles.size()) {
		graphics.paint(titlePositions[titlesPointer], 0, titles[titlesPointer].size(), 'w');
		graphics.paintBG(titlePositions[titlesPointer], 0, titles[titlesPointer].size(), 'n');
		titlesPointer = titleP;
		graphics.paint(titlePositions[titlesPointer], 0, titles[titlesPointer].size(), 'n');
		graphics.paintBG(titlePositions[titlesPointer], 0, titles[titlesPointer].size(), 'w');
	}

	// Itens
	int sobre = 1;
	for (list<Concept*>::iterator it = inventory[titlesPointer].begin(); it != inventory[titlesPointer].end(); it++, sobre++) {
		string s = (*it)->getName();
		sobre += s.size() / separator;
		graphics.drawString(0, sobre, s);
	}

	bool changedItem = invPointer != invP;

	if (0 < invP) {
		graphics.paint(0, invPointer, separator, 'w');
		graphics.paintBG(0, invPointer, separator, 'n');
		if (invP <= inventory[titlesPointer].size()) {
			invPointer = invP;
		}
		if (paintItem && (invP <= inventory[titlesPointer].size() || changedItem)) {
			graphics.paint(0, invPointer, separator, 'n');
			graphics.paintBG(0, invPointer, separator, 'w');
		}
	}

}

void Interface::consoleInterface() {
	// Current line
	string s = currentLine;
	graphics.drawString(separator + 1, screenHeight - 1, s);
	graphics.paint(separator + 1, screenHeight - 1, s.size() + 1, 'c');
	underlineInterface(underline);

	// Lines
	int sobre = 1;
	for (it = lines.begin(); it != lines.end(); it++, sobre++) {
		s = *it;
		sobre += s.size() / (screenWidth - separator);
		graphics.drawString(separator + 1, screenHeight - sobre - 1, s);
	}
}

void Interface::underlineInterface(bool n) {
	underline = n;
	if (underline) {
		if (pointer < currentLine.size()) {
			graphics.paintBG(separator + pointer + 1, screenHeight - 1, 1, 'c');
			graphics.paint(separator + pointer + 1, screenHeight - 1, 1, 'n');
		}
		else
			graphics.draw(separator + pointer + 1, screenHeight - 1, '_');
	}
	else
		if (pointer < currentLine.size()) {
			graphics.paintBG(separator + pointer + 1, screenHeight - 1, 1, 'n');
			graphics.paint(separator + pointer + 1, screenHeight - 1, 1, 'c');
		}
		else
			graphics.draw(separator + pointer + 1, screenHeight - 1, ' ');
}



// EXECUÇÃO -------------------------------

void Interface::gameLoop() {
	while (1) {
		time = (int)timeGetTime();
		clocking();
		time = (int)timeGetTime() - time;
		if (frameDelay > time)
			Sleep(frameDelay - time);
	}
}

void Interface::clocking() {
	input.input();

	(this->clock)++;

	// Underline pointer
	if (!textTab && this->clock % ULCOOLDOWN == 0) {
		underlineInterface(!underline);
		graphics.update();
		(this->clock) = 0;
	}

	// Menu progress
	if (menu && this->clock % MENUANIMATION == 0) {
		menuProgress++;
		menuInterface();
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
	mainInterface();
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
		removeLetter(true);

	if (input.getInput(input.deleter))
		removeLetter(false);

	if (input.getInput(input.enter)) {
		if (!menu) {
			args = riseLine();
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
		addLetter(input.getTyped());
}


void Interface::pointerLeft() {
	if (menu)
		return;

	if (textTab)
		setTitulosPointer(titlesPointer - 1);
	else
		setPointer(pointer - 1);
}

void Interface::pointerRight() {
	if (menu)
		return;

	if (textTab)
		setTitulosPointer(titlesPointer +1);
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

	addLetter(' ');
	underline = true;
}

void Interface::setTab(boolean tab) {
	if (menu)
		return;

	this->textTab = tab;
	inventoryInterface(invPointer, titlesPointer, textTab);
}

void Interface::setMenu(boolean menu) {
	setMenuPointer(MENU_OPTIONS.size());
	if (menu == false) {
		menuInterface();
	}

	this->menu = menu;
	this->menuProgress = 0;
	inventoryInterface(invPointer, titlesPointer, !menu);
}

void Interface::setConcepts(vector<Concept*> concepts, char type) {
	int index;
	for (index = 0; index < titles.size(); index++)
		if (titles[index][0] == toupper(type))
			break;

	if (concepts.size() > inventory[index].size())
		for (int i = 0; i < concepts.size(); i++) {
			if (i >= inventory[index].size()) {
				inventory[index].push_back(concepts[i]);
			}
		}
	else if (concepts.size() < inventory[index].size()) {
		inventory[index].clear();
		for (int i = 0; i < concepts.size(); i++) {
			inventory[index].push_back(concepts[i]);
		}
	}

	inventoryInterface(invPointer, titlesPointer, false);
}

void Interface::addLetter(char nova) {
	if (textTab || menu)
		return;

	currentLine = currentLine.substr(0, pointer) + nova + currentLine.substr(pointer, currentLine.length());
	setPointer(pointer + 1);
}

void Interface::removeLetter(bool before) {
	if (textTab || menu)
		return;

	if (before && pointer > 0) {
		currentLine = currentLine.substr(0, pointer-1) + currentLine.substr(pointer, currentLine.length());
		setPointer(pointer - 1);
	}

	else if (pointer < currentLine.length()) {
		currentLine = currentLine.substr(0, pointer) + currentLine.substr(pointer + 1, currentLine.length());
	}

	underline = true;
}


vector<string> Interface::riseLine() {
	vector<string> retorno = splitString(currentLine, ' ');

	if (currentLine == "" || menu)
		return retorno;

	sentLines.push_front(currentLine);
	if (sentLines.size() > screenHeight - 1)
		sentLines.pop_back();

	printLine(currentLine);
	savedLine = "";
	setVPointer(0);
	setPointer(0);
	return retorno;
}


void Interface::printLine(string newLine) {
	vector<string> novaCortada = splitString(newLine, '\n');
	int consoleSize = screenWidth - separator - 2;

	for (int i = 0; i < novaCortada.size(); i++) {
		if (novaCortada[i].size() > consoleSize) {
			string strInteira = novaCortada[i];
			novaCortada.emplace(novaCortada.begin() + i + 1, 
								novaCortada[i].substr(min(consoleSize+1, novaCortada[i].find_last_of(' ', consoleSize)+1), string::npos));
			novaCortada.emplace(novaCortada.begin() + i + 1, 
								novaCortada[i].substr(0, min(consoleSize, novaCortada[i].find_last_of(' ', consoleSize))));
			novaCortada.erase(novaCortada.begin() + i);
		}
	}
	for (int i = 0; i < novaCortada.size(); i++) {
		lines.push_front(novaCortada[i]);
		if (lines.size() > screenHeight - 1)
			lines.pop_back();
	}
}