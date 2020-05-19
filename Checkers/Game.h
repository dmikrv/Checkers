#pragma once
#include <Windows.h>
#include <iostream>
#include "Options.h"
#include "Console.h"

void initField(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], int gameMode);
void rotateField(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH]);
void rotatePlayer(int& sideNow, int& sideNowCol);
void takingChecker(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj, COORD target);

void drawField(HANDLE hOut, unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], short X, short Y);
void drawCount(HANDLE hOut, unsigned int countBl, unsigned int countWh);
void drawSideshow(HANDLE hOut, int sideNow, short X, short Y);
void drawNumbering(HANDLE h, short X, short Y);
void drawLegendMap(HANDLE hOut, short X, short Y);

bool isObligatoryMove(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], int sideNow);
bool isAllowedMove(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj, COORD target);
bool isAllowedMove(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj);
bool isAllowedBeat(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj, COORD target, int sideNow);
bool isAllowedBeat(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj, int sideNow);
bool isHaveAct(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], int sideNow);

int module(int a);
int whoseObject(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj);
int getInversionSide(int sideNow);
COORD calculateGradient(COORD obj, COORD target);

void endGame(HANDLE hOut, int sideWin);

const int SIDE_WHITE_COLOR = Colors::COLOR_WHITE; // цвет белого
const int SIDE_BLACK_COLOR = Colors::COLOR_GRAY; // цвет черного
const int SELECTED_COLOR = Colors::COLOR_BLUE;

enum Obj {
	OBJ_EMPTY,
	OBJ_WHITE,
	OBJ_BLACK,
	OBJ_WHITE_KING,
	OBJ_BLACK_KING,
};

enum Side {
	SIDE_WHITE,
	SIDE_BLACK,
};

enum GameMode {
	GM_WHITE,
	GM_BLACK,
	GM_WATCHER,
	GM_TWOPLAYER,
};