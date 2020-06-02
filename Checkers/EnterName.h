#pragma once
#include <iostream>
#include <Windows.h>
#include "Checkers.h"
#include "Console.h"
#include "Sounds.h"

void enterPlayersName(HANDLE& hOut, HANDLE& hIn, char* playerName1, char* playerName2, int buffSize, int* result);