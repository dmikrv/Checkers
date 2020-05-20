#pragma once
#include "Checkers.h"
#include "Console.h"

void game(HANDLE& hOut, HANDLE& hIn, int gameMode, int* result);
void endGame(HANDLE& hOut, HANDLE& hIn, int sideWin, int* result);

void initField(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], int gameMode);
void rotateField(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH]);
void rotatePlayer(int& sideNow, int& sideNowColor);
void takingChecker(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj, COORD target);

void drawField(HANDLE& hOut, unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], short X, short Y);
void drawCount(HANDLE& hOut, unsigned int countBl, unsigned int countWh);
void drawSideshow(HANDLE& hOut, int sideNow, short X, short Y);
void drawNumbering(HANDLE& hOut, short X, short Y);
void drawLegendMap(HANDLE& hOut, short X, short Y);
void drawButtons(HANDLE& hOut, short X, short Y);

bool isObligatoryMove(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], int sideNow);
bool isAllowedMove(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj, COORD target);
bool isAllowedMove(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj);
bool isAllowedBeat(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj, COORD target, int sideNow);
bool isAllowedBeat(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj, int sideNow);
bool isHaveAct(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], int sideNow);

int whoseObject(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj);
int getInversionSide(int sideNow);
COORD calculateGradient(COORD obj, COORD target);

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