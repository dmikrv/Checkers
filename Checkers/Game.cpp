#include "Game.h"

template <typename T> void mySwap(T& a, T& b)
{
    T temp = b;
    b = a;
    a = temp;
}

int module(int a)
{
    if (a > 0)
        return a;
    else if (a < 0)
        return -a;
    else
        return 0;
}

void initField(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], int gameMode)
{
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (i >= 0 && i < 3 && (i + j) % 2 == 1) {
                gameMode == GM_BLACK ? field[i][j] = OBJ_WHITE : field[i][j] = OBJ_BLACK;
                //gameMode == GM_BLACK ? field[i][j] = OBJ_WHITE : field[i][j] = OBJ_BLACK_KING;

            }
            else if (i >= 5 && i < 8 && (i + j) % 2 == 1) {
                gameMode == GM_BLACK ? field[i][j] = OBJ_BLACK : field[i][j] = OBJ_WHITE;
                //gameMode == GM_BLACK ? field[i][j] = OBJ_BLACK : field[i][j] = OBJ_WHITE_KING;

            }
            else {
                field[i][j] = OBJ_EMPTY;
            }
        }
    }
}

void drawField(HANDLE hOut, unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], short X, short Y)
{
    for (short i = 0; i < FIELD_HEIGHT; i++) {
        for (short j = 0; j < FIELD_WIDTH; j++) {
            SetConsoleCursorPosition(hOut, { j + X, i + Y });
            if (field[i][j] == OBJ_EMPTY && (i + j) % 2 == 1) {
                SetConsoleTextAttribute(hOut, Colors::COLOR_DARKYELLOW);
                std::cout << char(176);
            }
            else if (field[i][j] == OBJ_EMPTY) {
                std::cout << " ";
            }
            if (field[i][j] == OBJ_WHITE) {
                SetConsoleTextAttribute(hOut, SIDE_WHITE_COLOR);
                std::cout << char(177);
            }
            else if (field[i][j] == OBJ_BLACK) {
                SetConsoleTextAttribute(hOut, SIDE_BLACK_COLOR);
                std::cout << char(177);
            }
            else if (field[i][j] == OBJ_WHITE_KING) {
                SetConsoleTextAttribute(hOut, SIDE_WHITE_COLOR);
                std::cout << char(178);
            }
            else if (field[i][j] == OBJ_BLACK_KING) {
                SetConsoleTextAttribute(hOut, SIDE_BLACK_COLOR);
                std::cout << char(178);
            }

        }
    }
}

void drawCount(HANDLE hOut, unsigned int countBl, unsigned int countWh)
{
    SetConsoleCursorPosition(hOut, { SHOW_POS_X, SHOW_POS_Y + 4 });
    SetConsoleTextAttribute(hOut, Colors::COLOR_WHITE);
    std::cout << "WHITE: " << countWh << " ";
    SetConsoleCursorPosition(hOut, { SHOW_POS_X, SHOW_POS_Y + 5 });
    std::cout << "BLACK: " << countBl << " ";
}

void drawSideshow(HANDLE hOut, int sideNow, short X, short Y)
{
    SetConsoleCursorPosition(hOut, { X, Y });
    SetConsoleTextAttribute(hOut, Colors::COLOR_WHITE);
    if (sideNow == SIDE_WHITE)
        std::cout << "WHITE MOVE";
    else
        std::cout << "BLACK MOVE";
}

void drawNumbering(HANDLE h, short X, short Y)
{
    SetConsoleTextAttribute(h, Colors::COLOR_WHITE);
    for (short i = 0; i < 10; i++) {
        for (short j = 0; j < 10; j++) {
            SetConsoleCursorPosition(h, { j + X, i + Y});
            if (i == 0 || i == 9) {
                std::cout << " ABCDEFGH ";
                break;
            }
            if (j == 0 || j == 9) {
                std::cout << i;

            }
        }
    }
}

void drawLegendMap(HANDLE hOut, short X, short Y)
{
    SetConsoleCursorPosition(hOut, { X, Y });
    std::cout << "Legend map: " << std::endl;

    SetConsoleCursorPosition(hOut, { X, Y + 1 });
    SetConsoleTextAttribute(hOut, SIDE_WHITE_COLOR);
    std::cout << char(177) << " - white, ";
    std::cout << char(178) << " - white king";

    SetConsoleCursorPosition(hOut, { X, Y + 2 });
    SetConsoleTextAttribute(hOut, SIDE_BLACK_COLOR);
    std::cout << char(177) << " - black, ";
    std::cout << char(178) << " - black king";
}

bool isAllowedBeat(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj, COORD target, int sideNow)
{
    if ((field[obj.Y][obj.X] == OBJ_WHITE || field[obj.Y][obj.X] == OBJ_BLACK) && field[target.Y][target.X] == OBJ_EMPTY
        && module(target.Y - obj.Y) == 2 && module(target.X - obj.X) == 2
        && whoseObject(field, { obj.X + (target.X - obj.X) / 2 , obj.Y + (target.Y - obj.Y) / 2 })
        == getInversionSide(sideNow)) {
        return true;
    }
    else if ((field[obj.Y][obj.X] == OBJ_WHITE_KING || field[obj.Y][obj.X] == OBJ_BLACK_KING)
        && field[target.Y][target.X] == OBJ_EMPTY && module(target.X - obj.X) == module(target.Y - obj.Y)) {
        int _const = module(target.X - obj.X);
        int countEnemyChecker = 0;
        COORD grad{ (target.X - obj.X) / _const, (target.Y - obj.Y) / _const }; // градиент, напрвление движения
        for (short i = 1; i < _const; i++) {
            int object = whoseObject(field, { i * grad.X + obj.X, i * grad.Y + obj.Y });
            if (object == getInversionSide(sideNow)) {
                countEnemyChecker++;
            }
            else if (object == sideNow) {
                return false;
            }
        }
        if (countEnemyChecker == 1)
            return true;
    }
    return false;
}

bool isAllowedBeat(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj, int sideNow)
{
    // если это шашка
    if (field[obj.Y][obj.X] == OBJ_WHITE || field[obj.Y][obj.X] == OBJ_BLACK) {
        if (whoseObject(field, { obj.X - 1, obj.Y - 1 }) == getInversionSide(sideNow)
            && field[obj.Y - 2][obj.X - 2] == OBJ_EMPTY && obj.Y - 2 >= 0 && obj.X - 2 >= 0
            || whoseObject(field, { obj.X + 1, obj.Y - 1 }) == getInversionSide(sideNow)
            && field[obj.Y - 2][obj.X + 2] == OBJ_EMPTY && obj.Y - 2 >= 0 && obj.X + 2 < FIELD_WIDTH
            || whoseObject(field, { obj.X - 1, obj.Y + 1 }) == getInversionSide(sideNow)
            && field[obj.Y + 2][obj.X - 2] == OBJ_EMPTY && obj.Y + 2 < FIELD_HEIGHT && obj.X - 2 >= 0
            || whoseObject(field, { obj.X + 1, obj.Y + 1 }) == getInversionSide(sideNow)
            && field[obj.Y + 2][obj.X + 2] == OBJ_EMPTY && obj.Y + 2 < FIELD_HEIGHT && obj.X + 2 < FIELD_WIDTH) {
            return true;
        }
    }
    // если дамка
    else {
        COORD grad[4] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
        for (short i = 0; i < 4; i++) {
            for (short j = 1;  obj.X + grad[i].X * j >= 1 && obj.X + grad[i].X * j < FIELD_WIDTH - 1
                && obj.Y + grad[i].Y * j >= 1 && obj.Y + grad[i].Y * j < FIELD_HEIGHT - 1; j++) {
                if (whoseObject(field, { obj.X + grad[i].X * j, obj.Y + grad[i].Y * j }) == sideNow) {
                    break;
                }
                else if (whoseObject(field, { obj.X + grad[i].X * j, obj.Y + grad[i].Y * j}) 
                    == getInversionSide(sideNow)) {
                    if (field[obj.Y + grad[i].Y * (j + 1)][obj.X + grad[i].X * (j + 1)] == OBJ_EMPTY)
                        return true;
                    else
                        break;
                }
            }
        }
    }

    return false;
}

bool isObligatoryMove(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], int sideNow)
{
    for (short i = 0; i < FIELD_HEIGHT; i++) {
        for (short j = 0; j < FIELD_WIDTH; j++) {
            if (whoseObject(field, { j, i }) == sideNow) {
                if (isAllowedBeat(field, { j, i }, sideNow) == true) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool isAllowedMove(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj)
{
    /////////////////////// ЗАПЛАТКА. ПОТОМ ПОДУМАТЬ /////////////////////////
    if (field[obj.Y][obj.X] == OBJ_WHITE_KING || field[obj.Y][obj.X] == OBJ_BLACK_KING) {
        return true;
    }
    else if (field[obj.Y - 1][obj.X - 1] == OBJ_EMPTY || field[obj.Y - 1][obj.X + 1] == OBJ_EMPTY
        && field[obj.Y + 1][obj.X - 1] == OBJ_EMPTY || field[obj.Y + 1][obj.X + 1] == OBJ_EMPTY) {
        return true;
    }
    return false;
}

bool isAllowedMove(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj, COORD target)
{
    // если наша цель пуста
    if (field[target.Y][target.X] == OBJ_EMPTY 
        && target.X >= 0 && target.X < FIELD_WIDTH && target.Y >= 0 && target.Y < FIELD_HEIGHT) {

        // если это шашка
        if (field[obj.Y][obj.X] == OBJ_WHITE || field[obj.Y][obj.X] == OBJ_BLACK) {
            if (target.Y == obj.Y - 1 && (target.X == obj.X - 1 || target.X == obj.X + 1)) {
                return true;
            }
        }

        // если это дамка
        if ((field[obj.Y][obj.X] == OBJ_WHITE_KING || field[obj.Y][obj.X] == OBJ_BLACK_KING)
            && module(target.X - obj.X) == module(target.Y - obj.Y)) {
            int _const = module(target.X - obj.X);
            COORD grad = calculateGradient(obj, target); // градиент, напрвление движения
            int sideNow = whoseObject(field, obj);
            for (short i = 1; i < _const; i++) {
                if (field[i * grad.Y + obj.Y][i * grad.X + obj.X] != OBJ_EMPTY) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

void rotatePlayer(int& sideNow, int& sideNowCol)
{
    sideNow = getInversionSide(sideNow);
    sideNowCol = (sideNow == SIDE_WHITE ? SIDE_WHITE_COLOR : SIDE_BLACK_COLOR);
}

void rotateField(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH])
{
    for (short i = 0; i < FIELD_HEIGHT / 2; i++) {
        for (short j = 0; j < FIELD_WIDTH; j++) {
            mySwap(field[i][j], field[FIELD_HEIGHT - i - 1][FIELD_WIDTH - j - 1]);
        }
    }
} 

bool isHaveAct(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], int sideNow)
{
    for (short i = 0; i < FIELD_HEIGHT; i++) {
        for (short j = 0; j < FIELD_WIDTH; j++) {
            if (whoseObject(field, { j, i }) == sideNow) {
                if (isAllowedMove(field, { j, i }) || isAllowedBeat(field, { j, i }, sideNow))
                    return true;
            }
        }
    }
    return false;
}

int whoseObject(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj)
{
    if (field[obj.Y][obj.X] == OBJ_WHITE || field[obj.Y][obj.X] == OBJ_WHITE_KING)
        return SIDE_WHITE;
    else if (field[obj.Y][obj.X] == OBJ_BLACK || field[obj.Y][obj.X] == OBJ_BLACK_KING)
        return SIDE_BLACK;
    else
        return -1;
}

inline int getInversionSide(int sideNow)
{
    return sideNow == SIDE_WHITE ? SIDE_BLACK : SIDE_WHITE;
}

COORD calculateGradient(COORD obj, COORD target)
{
    if (module(target.X - obj.X) != module(target.Y - obj.Y))
        Sleep(INFINITE);
    int _const = module(target.X - obj.X);
    COORD grad{ (target.X - obj.X) / _const, (target.Y - obj.Y) / _const }; // градиент, напрвление движения
    return grad;
}

void takingChecker(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj, COORD target)
{
    int sideNow = whoseObject(field, obj);
    int _const = module(target.X - obj.X);
    COORD grad = calculateGradient(obj, target);

    for (short i = 1; i < _const; i++) {
        if (whoseObject(field, { i * grad.X + obj.X, i * grad.Y + obj.Y }) == getInversionSide(sideNow)) {
            field[i * grad.Y + obj.Y][i * grad.X + obj.X] = OBJ_EMPTY;
        }
    }

    field[target.Y][target.X] = field[obj.Y][obj.X];
    field[obj.Y][obj.X] = OBJ_EMPTY;
}


void endGame(HANDLE hOut, int sideWin)
{
    Sleep(1000);
    system("CLS");
    SetConsoleCursorPosition(hOut, { 2, 2 });
    SetConsoleTextAttribute(hOut, Colors::COLOR_WHITE);
    if (sideWin == SIDE_WHITE) {
        std::cout << "WHITE WIN";
    }
    else {
        std::cout << "BLACK WIN";
    }

    system("pause>NUL");
}