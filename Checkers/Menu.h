#pragma once

#include <Windows.h>
#include <iostream>
#include "Console.h"
#include "Game.h"

enum MenuMode {
	MENU_MAIN,
	MENU_GM_SELECTION,
	MEBU_SIDE_SELECTION,
};

void menu(HANDLE& hOut, HANDLE& hIn, int* gameMode);

void drawLogo(HANDLE& hOut);
void drawMenu(HANDLE& hOut, HANDLE& hIn, COORD* button, int mode);