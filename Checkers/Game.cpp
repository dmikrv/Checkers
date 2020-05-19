#include "Game.h"

void game(HANDLE& hOut, HANDLE& hIn, int gameMode, int* result)
{
    unsigned char field[FIELD_HEIGHT][FIELD_WIDTH];
    register int sideNow = (gameMode == GM_BLACK ? SIDE_BLACK : SIDE_WHITE);
    int sideNowCol = (sideNow == SIDE_WHITE ? SIDE_WHITE_COLOR : SIDE_BLACK_COLOR);
    initField(field, gameMode);

    INPUT_RECORD all_events;
    DWORD cWrittenChars;
    DWORD count;
    COORD point;

    unsigned countBlack = 12;
    unsigned countWhite = 12;

    bool doneFlag = false; // флаг сделанного хода

    bool selectedFlag = false;
    COORD selectedCoord{ 0, 0 };

    bool beatFlag = false;
    COORD beatCoord{ 0, 0 };

    system("cls");
    SetConsoleMode(hIn, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    //changeFont(hOut, { WINDOW_FONT_SIZE_WIDTH , WINDOW_FONT_SIZE_HEIGHT });
    if (gameMode == GM_TWOPLAYER)
        drawSideshow(hOut, sideNow, SIDESHOW_POS_X, SIDESHOW_POS_Y);
    drawNumbering(hOut, NUMBERING_POS_X, NUMBERING_POS_Y);
    drawLegendMap(hOut, LEGENDMAP_POS_X, LEGENDMAP_POS_Y);
    drawField(hOut, field, FIELD_POS_X, FIELD_POS_Y);
    drawCount(hOut, countBlack, countWhite);
    drawButtons(hOut, BUTTONS_POS_X, BUTTONS_POS_Y);

    while (true) {
        ReadConsoleInput(hIn, &all_events, 1, &count);
        point.X = all_events.Event.MouseEvent.dwMousePosition.X;
        point.Y = all_events.Event.MouseEvent.dwMousePosition.Y;

        // получение координат в рамках игрового поля
        COORD pointC = { point.X - FIELD_POS_X, point.Y - FIELD_POS_Y }; // point cut
        COORD selectedCoordC = { selectedCoord.X - FIELD_POS_X, selectedCoord.Y - FIELD_POS_Y }; // selected cut

        // new game
        if (point.X >= BUTTONS_POS_X && point.X < BUTTONS_POS_X + 8 && point.Y == BUTTONS_POS_Y) {
            FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 8, 
                { BUTTONS_POS_X, BUTTONS_POS_Y }, &cWrittenChars);
            if (all_events.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                *result = 0;
                return;
            }
        }
        else {
            FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 8,
                { BUTTONS_POS_X, BUTTONS_POS_Y }, &cWrittenChars);
        }

        // menu
        if (point.X >= BUTTONS_POS_X + 2 && point.X < BUTTONS_POS_X + 6 && point.Y == BUTTONS_POS_Y + 1) {
            FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 4,
                { BUTTONS_POS_X + 2, BUTTONS_POS_Y + 1 }, &cWrittenChars);
            if (all_events.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                *result = 1;
                return;
            }
        }
        else {
            FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 4,
                { BUTTONS_POS_X + 2, BUTTONS_POS_Y + 1 }, &cWrittenChars);
        }

        if (all_events.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
            // проверка на отсутствие ходов
            if (isHaveAct(field, sideNow) == false) {
                endGame(hOut, hIn, getInversionSide(sideNow), result);
                return;
            }
            // выбор шашки для управления ею
            if (whoseObject(field, pointC) == sideNow
                && point.X >= FIELD_POS_X && point.X < FIELD_POS_X + FIELD_WIDTH
                && point.Y >= FIELD_POS_Y && point.Y < FIELD_POS_Y + FIELD_HEIGHT
                && (point.X != selectedCoord.X || point.Y != selectedCoord.Y)
                && (!beatFlag || pointC.X == beatCoord.X && pointC.Y == beatCoord.Y)) {
                if (selectedFlag == true) {
                    FillConsoleOutputAttribute(hOut, sideNowCol, 1, selectedCoord, &cWrittenChars);
                }
                FillConsoleOutputAttribute(hOut, SELECTED_COLOR, 1, point, &cWrittenChars);
                selectedCoord.X = point.X;
                selectedCoord.Y = point.Y;
                selectedFlag = true;
            }
            // бить шашкой
            if (isObligatoryMove(field, sideNow) == true) {
                if (isAllowedBeat(field, selectedCoordC, pointC, sideNow) == true && selectedFlag == true
                    && pointC.X >= 0 && pointC.X < FIELD_WIDTH && pointC.Y >= 0 && pointC.Y < FIELD_HEIGHT) {
                    selectedFlag = false;
                    selectedCoord = { 0, 0 };

                    takingChecker(field, selectedCoordC, pointC);

                    if (sideNow == SIDE_WHITE)
                        countBlack--;
                    else
                        countWhite--;

                    if (pointC.Y == 0)
                        field[pointC.Y][pointC.X] = (sideNow == SIDE_WHITE ? OBJ_WHITE_KING : OBJ_BLACK_KING);

                    if (isAllowedBeat(field, pointC, sideNow)) {
                        beatFlag = true;
                        beatCoord = { pointC.X, pointC.Y };
                        drawField(hOut, field, FIELD_POS_X, FIELD_POS_Y);
                        drawCount(hOut, countBlack, countWhite);
                    }
                    else {
                        doneFlag = true;
                        beatFlag = false;
                        beatCoord = { 0, 0 };
                    }
                }
            }
            // перемещение шашки или дамки
            else if (isAllowedMove(field, selectedCoordC, pointC) && selectedFlag == true) {
                selectedFlag = false;
                selectedCoord = { 0, 0 };
                field[pointC.Y][pointC.X] = field[selectedCoordC.Y][selectedCoordC.X];
                field[selectedCoordC.Y][selectedCoordC.X] = OBJ_EMPTY;
                if (pointC.Y == 0)
                    field[pointC.Y][pointC.X] = (sideNow == SIDE_WHITE ? OBJ_WHITE_KING : OBJ_BLACK_KING);

                doneFlag = true;
            }

            // отмена действия
            if ((point.X != selectedCoord.X || point.Y != selectedCoord.Y) && selectedFlag == true) {
                FillConsoleOutputAttribute(hOut, sideNowCol, 1, selectedCoord, &cWrittenChars);
                selectedFlag = false;
                selectedCoord = { 0, 0 };
            }
            else if (all_events.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED && selectedFlag == true) {

            }
            // проверка на сделанный ход
            if (doneFlag && !beatFlag) {
                doneFlag = false;
                drawField(hOut, field, FIELD_POS_X, FIELD_POS_Y);
                drawCount(hOut, countBlack, countWhite);

                // проверка на наличие шашек
                if ((sideNow == SIDE_WHITE && countBlack == 0)
                    || (sideNow == SIDE_BLACK && countWhite == 0)) {
                    endGame(hOut, hIn, getInversionSide(sideNow), result);
                    return;
                }

                if (gameMode == GM_TWOPLAYER) {
                    rotatePlayer(sideNow, sideNowCol);
                    rotateField(field);
                    Sleep(450);
                    drawField(hOut, field, FIELD_POS_X, FIELD_POS_Y);
                    drawSideshow(hOut, sideNow, SIDESHOW_POS_X, SIDESHOW_POS_Y);
                }             
            }
        }

        // отмена действия
        if (all_events.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED && selectedFlag == true) {
            FillConsoleOutputAttribute(hOut, sideNowCol, 1, selectedCoord, &cWrittenChars);
            selectedFlag = false;
            selectedCoord = { 0, 0 };
        }

        //if (all_events.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
        //    SetConsoleCursorPosition(h, point);
        //    cout << (char)219; 
        //}
        //else if (all_events.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
        //    SetConsoleCursorPosition(h, point);
        //    cout << " ";
        //}

        //if (all_events.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED 
        //    && point.X >= 20 && point.X < 40 && point.Y >= 10 && point.Y < 13) {     
        //    b(h, ++color);
        //}

        //else if (all_events.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED
        //    && point.X >= 20 && point.X < 40 && point.Y >= 20 && point.Y < 23) {
        //    c(h, ++color2);
        //}
    }
}

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

void drawField(HANDLE &hOut, unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], short X, short Y)
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

void drawCount(HANDLE &hOut, unsigned int countBl, unsigned int countWh)
{
    SetConsoleCursorPosition(hOut, { SHOW_POS_X, SHOW_POS_Y + 4 });
    SetConsoleTextAttribute(hOut, Colors::COLOR_WHITE);
    std::cout << "WHITE: " << countWh << " ";
    SetConsoleCursorPosition(hOut, { SHOW_POS_X, SHOW_POS_Y + 5 });
    std::cout << "BLACK: " << countBl << " ";
}

void drawSideshow(HANDLE &hOut, int sideNow, short X, short Y)
{
    SetConsoleCursorPosition(hOut, { X, Y });
    SetConsoleTextAttribute(hOut, Colors::COLOR_YELLOW);
    if (sideNow == SIDE_WHITE)
        std::cout << "WHITE MOVE";
    else
        std::cout << "BLACK MOVE";
}

void drawNumbering(HANDLE &hOut, short X, short Y)
{
    SetConsoleTextAttribute(hOut, Colors::COLOR_WHITE);
    for (short i = 0; i < 10; i++) {
        for (short j = 0; j < 10; j++) {
            SetConsoleCursorPosition(hOut, { j + X, i + Y});
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

void drawLegendMap(HANDLE &hOut, short X, short Y)
{
    SetConsoleCursorPosition(hOut, { X, Y });
    SetConsoleTextAttribute(hOut, Colors::COLOR_WHITE);
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

void drawButtons(HANDLE& hOut, short X, short Y)
{
    SetConsoleCursorPosition(hOut, { X, Y });
    SetConsoleTextAttribute(hOut, Colors::COLOR_GRAY);
    std::cout << "NEW GAME";
    Y += 1;
    X += 2;
    SetConsoleCursorPosition(hOut, { X, Y });
    std::cout << "MENU";
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
        short _const = module(target.X - obj.X);
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
                else if (whoseObject(field, { obj.X + grad[i].X * j,
                    obj.Y + grad[i].Y * j }) == getInversionSide(sideNow)) {
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
            if (whoseObject(field, { j, i }) == sideNow && isAllowedBeat(field, { j, i }, sideNow) == true) {
                return true;
            }
        }
    }
    return false;
}

bool isAllowedMove(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj)
{
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
    short _const = module(target.X - obj.X);
    COORD grad{ (target.X - obj.X) / _const, (target.Y - obj.Y) / _const }; // градиент, напрвление движения
    return grad;
}

void takingChecker(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj, COORD target)
{
    int sideNow = whoseObject(field, obj);
    short _const = module(target.X - obj.X);
    COORD grad = calculateGradient(obj, target);

    for (short i = 1; i < _const; i++) {
        if (whoseObject(field, { i * grad.X + obj.X, i * grad.Y + obj.Y }) == getInversionSide(sideNow)) {
            field[i * grad.Y + obj.Y][i * grad.X + obj.X] = OBJ_EMPTY;
        }
    }

    field[target.Y][target.X] = field[obj.Y][obj.X];
    field[obj.Y][obj.X] = OBJ_EMPTY;
}

void endGame(HANDLE &hOut, HANDLE &hIn, int sideWin, int* result)
{
    Sleep(1000);
    system("CLS");
    SetConsoleMode(hIn, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    SetConsoleCursorPosition(hOut, { WINDOW_COLS / 3, WINDOW_LINES / 4 });
    SetConsoleTextAttribute(hOut, Colors::COLOR_WHITE);
    if (sideWin == SIDE_WHITE) {
        std::cout << "WHITE WIN!";
    }
    else {
        std::cout << "BLACK WIN!";
    }

    INPUT_RECORD all_events;
    DWORD count;
    COORD point;
    DWORD cWrittenChars;

    COORD buttNewgame{ WINDOW_COLS / 2 - 4, 6 };
    COORD buttMenu{ WINDOW_COLS / 2 - 2, 7 };
    COORD buttExit{ WINDOW_COLS / 2 - 2, 8 };

    SetConsoleTextAttribute(hOut, Colors::COLOR_GRAY);
    SetConsoleCursorPosition(hOut, buttNewgame);
    std::cout << "NEW GAME";
    SetConsoleCursorPosition(hOut, buttMenu);
    std::cout << "MENU";
    SetConsoleCursorPosition(hOut, buttExit);
    std::cout << "EXIT";

    while (true) {
        ReadConsoleInput(hIn, &all_events, 1, &count);
        point.X = all_events.Event.MouseEvent.dwMousePosition.X;
        point.Y = all_events.Event.MouseEvent.dwMousePosition.Y;

        if (all_events.EventType == MOUSE_EVENT) {
            // new game
            if (point.Y == buttNewgame.Y && point.X >= buttNewgame.X && point.X < buttNewgame.X + 8) {
                FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 8, buttNewgame, &cWrittenChars);
                if (all_events.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                    *result = 0;
                    return;
                }
            } 
            else {
                FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 8, buttNewgame, &cWrittenChars);
            }
                
            // menu
            if (point.Y == buttMenu.Y && point.X >= buttMenu.X && point.X < buttMenu.X + 4) {
                FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 4, buttMenu, &cWrittenChars);
                if (all_events.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                    *result = 1;
                    return;
                }
            }       
            else {
                FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 4, buttMenu, &cWrittenChars);
            }
                
            // exit
            if (point.Y == buttExit.Y && point.X >= buttExit.X && point.X < buttExit.X + 4) {
                FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 4, buttExit, &cWrittenChars);
                if (all_events.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                    exit(0);
                }
            } 
            else {
                FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 4, buttExit, &cWrittenChars);
            }
        }
    }
}