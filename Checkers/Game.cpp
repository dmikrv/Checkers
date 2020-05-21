#include "Game.h"

void game(HANDLE& hOut, HANDLE& hIn, int gameMode, int* result)
{
    system("cls");
    SetConsoleMode(hIn, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    unsigned char field[FIELD_HEIGHT][FIELD_WIDTH];
    register int sideNow = (gameMode == GM_BLACK ? SIDE_BLACK : SIDE_WHITE);
    int sideNowColor = (sideNow == SIDE_WHITE ? SIDE_WHITE_COLOR : SIDE_BLACK_COLOR);
    initField(field, gameMode);

    INPUT_RECORD allEvents;
    DWORD cWrittenChars;
    DWORD count;
    COORD mousePoint;

    unsigned countBlack = 12;
    unsigned countWhite = 12;

    bool doneFlag = false; // флаг сделанного хода
    bool obligatoryFlag = false; // флаг обязательного хода
    COORD selectedObject{ -1, 0 }; // X=-1 -- object is not selected
    COORD beatCoord{ -1, 0 }; // X=-1 -- beat checker is not active

    if (gameMode == GM_TWOPLAYER)
        drawSideshow(hOut, sideNow, SIDESHOW_POS_X, SIDESHOW_POS_Y);
    drawNumbering(hOut, NUMBERING_POS_X, NUMBERING_POS_Y);
    drawLegendMap(hOut, LEGENDMAP_POS_X, LEGENDMAP_POS_Y);
    drawField(hOut, field, FIELD_POS_X, FIELD_POS_Y);
    drawCount(hOut, countBlack, countWhite);
    drawButtons(hOut, BUTTONS_POS_X, BUTTONS_POS_Y);

    while (true) {
        ReadConsoleInput(hIn, &allEvents, 1, &count);
        mousePoint.X = allEvents.Event.MouseEvent.dwMousePosition.X;
        mousePoint.Y = allEvents.Event.MouseEvent.dwMousePosition.Y;

        // получение координат в рамках игрового поля
        COORD mousePointRelative = getRelativePoint(mousePoint);
        COORD selectedObjectRelative = getRelativePoint(selectedObject);

        // new game
        if (mousePoint.X >= BUTTONS_POS_X && mousePoint.X < BUTTONS_POS_X + 8 && mousePoint.Y == BUTTONS_POS_Y) {
            FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 8, 
                { BUTTONS_POS_X, BUTTONS_POS_Y }, &cWrittenChars);
            if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                *result = 0;
                return;
            }
        }
        else {
            FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 8,
                { BUTTONS_POS_X, BUTTONS_POS_Y }, &cWrittenChars);
        }
        // menu
        if (mousePoint.X >= BUTTONS_POS_X + 2 && mousePoint.X < BUTTONS_POS_X + 6 
            && mousePoint.Y == BUTTONS_POS_Y + 1) {
            FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 4,
                { BUTTONS_POS_X + 2, BUTTONS_POS_Y + 1 }, &cWrittenChars);
            if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                *result = 1;
                return;
            }
        }
        else {
            FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 4,
                { BUTTONS_POS_X + 2, BUTTONS_POS_Y + 1 }, &cWrittenChars);
        }

        // проверка на отсутствие ходов
        if (isHaveAct(field, sideNow) == false) {
            endGame(hOut, hIn, getInversionSide(sideNow), result);
            return;
        }
        if (isObligatoryMove(field, sideNow) == true && !obligatoryFlag)
            obligatoryFlag = true;


        if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
            // выбор шашки для управления ею
            if (whoseObject(field, mousePointRelative) == sideNow
                && mousePoint.X >= FIELD_POS_X && mousePoint.X < FIELD_POS_X + FIELD_WIDTH
                && mousePoint.Y >= FIELD_POS_Y && mousePoint.Y < FIELD_POS_Y + FIELD_HEIGHT
                && (mousePoint.X != selectedObject.X || mousePoint.Y != selectedObject.Y)) {
                if ((!obligatoryFlag || isAllowedBeat(field, mousePointRelative))
                    && (mousePointRelative.X == beatCoord.X && mousePointRelative.Y == beatCoord.Y
                        || beatCoord.X == -1)) {
                    if (selectedObject.X != -1) {
                        FillConsoleOutputAttribute(hOut, sideNowColor, 1, selectedObject, &cWrittenChars);
                        showAllowedAction(hOut, field, selectedObjectRelative, true);
                    }
                    selectedObject = mousePoint;
                    showObligatoryChecker(hOut, field, sideNow, true);
                    FillConsoleOutputAttribute(hOut, SELECTED_CHECKER_COLOR, 1, mousePoint, &cWrittenChars);
                    showAllowedAction(hOut, field, mousePointRelative);
                }
                else {
                    if (beatCoord.X == -1)
                        showObligatoryChecker(hOut, field, sideNow);
                    else
                        FillConsoleOutputAttribute(hOut, HIGHLIGHT_CHECKER_COLOR,
                            1, getAbsolutePoint(beatCoord), &cWrittenChars);
                }
            }
                        
            // взятие другой шашки
            if (obligatoryFlag) {
                if (isAllowedBeat(field, selectedObjectRelative, mousePointRelative) == true 
                    && selectedObject.X != -1 && mousePointRelative.X >= 0 && mousePointRelative.X < FIELD_WIDTH 
                    && mousePointRelative.Y >= 0 && mousePointRelative.Y < FIELD_HEIGHT) {
                    selectedObject = { -1, 0 };

                    takingChecker(field, selectedObjectRelative, mousePointRelative);

                    if (sideNow == SIDE_WHITE)
                        countBlack--;
                    else
                        countWhite--;

                    if (mousePointRelative.Y == 0)
                        field[mousePointRelative.Y][mousePointRelative.X] = (sideNow == SIDE_WHITE ? 
                            OBJ_WHITE_KING : OBJ_BLACK_KING);

                    if (isAllowedBeat(field, mousePointRelative)) {
                        showAllowedAction(hOut, field, mousePointRelative, true);
                        drawField(hOut, field, FIELD_POS_X, FIELD_POS_Y);
                        drawCount(hOut, countBlack, countWhite);

                        beatCoord = mousePointRelative;
                        selectedObject = mousePoint;
                        FillConsoleOutputAttribute(hOut, SELECTED_CHECKER_COLOR, 1, mousePoint, &cWrittenChars);
                        showAllowedAction(hOut, field, mousePointRelative);
                    }
                    else {
                        doneFlag = true;
                        obligatoryFlag = false;
                        beatCoord = { -1, 0 };
                    }
                }
            }
            // перемещение шашки или дамки
            else if (isAllowedMove(field, selectedObjectRelative, mousePointRelative) && selectedObject.X != -1) {
                selectedObject = { -1, 0 };
                field[mousePointRelative.Y][mousePointRelative.X] = 
                    field[selectedObjectRelative.Y][selectedObjectRelative.X];

                field[selectedObjectRelative.Y][selectedObjectRelative.X] = OBJ_EMPTY;
                if (mousePointRelative.Y == 0)
                    field[mousePointRelative.Y][mousePointRelative.X] = 
                        (sideNow == SIDE_WHITE ? OBJ_WHITE_KING : OBJ_BLACK_KING);

                doneFlag = true;
            }

            // отмена действия
            if ((mousePoint.X != selectedObject.X || mousePoint.Y != selectedObject.Y) && selectedObject.X != -1) {
                FillConsoleOutputAttribute(hOut, sideNowColor, 1, selectedObject, &cWrittenChars);
                showAllowedAction(hOut, field, selectedObjectRelative, true);
                showObligatoryChecker(hOut, field, sideNow, true);
                selectedObject = { -1, 0 };
            }
            // проверка на сделанный ход
            if (doneFlag) {
                doneFlag = false;
                drawField(hOut, field, FIELD_POS_X, FIELD_POS_Y);
                drawCount(hOut, countBlack, countWhite);

                // проверка на наличие шашек
                if ((sideNow == SIDE_WHITE && countBlack == 0) || (sideNow == SIDE_BLACK && countWhite == 0)) {
                    endGame(hOut, hIn, sideNow, result);
                    return;
                }

                if (gameMode == GM_TWOPLAYER) {
                    rotatePlayer(sideNow, sideNowColor);
                    rotateField(field);
                    Sleep(ROTATE_FIELD_SLEEP);
                    drawField(hOut, field, FIELD_POS_X, FIELD_POS_Y);
                    drawSideshow(hOut, sideNow, SIDESHOW_POS_X, SIDESHOW_POS_Y);
                }             
            }
        }

        // отмена действия
        if (allEvents.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED && selectedObject.X != -1) {
            FillConsoleOutputAttribute(hOut, sideNowColor, 1, selectedObject, &cWrittenChars);
            showAllowedAction(hOut, field, selectedObjectRelative, true);
            showObligatoryChecker(hOut, field, sideNow, true);
            selectedObject = { -1, 0 };
        }
    }
}

template <typename T> void mySwap(T& a, T& b)
{
    T temp = b;
    b = a;
    a = temp;
}

template <typename T> T myAbs(T a)
{
    if (a >= 0)
        return a;
    else
        return -a;
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
                SetConsoleTextAttribute(hOut, CELL_COLOR);
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

bool isAllowedBeat(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj, COORD target)
{
    int sideNow = whoseObject(field, obj);
    if ((field[obj.Y][obj.X] == OBJ_WHITE || field[obj.Y][obj.X] == OBJ_BLACK) && field[target.Y][target.X] == OBJ_EMPTY
        && myAbs(target.Y - obj.Y) == 2 && myAbs(target.X - obj.X) == 2
        && whoseObject(field, { obj.X + (target.X - obj.X) / 2 , obj.Y + (target.Y - obj.Y) / 2 })
        == getInversionSide(sideNow)) {
        return true;
    }
    else if ((field[obj.Y][obj.X] == OBJ_WHITE_KING || field[obj.Y][obj.X] == OBJ_BLACK_KING)
        && field[target.Y][target.X] == OBJ_EMPTY && myAbs(target.X - obj.X) == myAbs(target.Y - obj.Y)) {
        short _const = myAbs(target.X - obj.X);
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

bool isAllowedBeat(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj)
{
    int sideNow = whoseObject(field, obj);
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
            if (whoseObject(field, { j, i }) == sideNow && isAllowedBeat(field, { j, i }) == true) {
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
            && myAbs(target.X - obj.X) == myAbs(target.Y - obj.Y)) {
            int _const = myAbs(target.X - obj.X);
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

void showObligatoryChecker(HANDLE& hOut, unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], int sideNow,
    bool isInversionAction)
{
    DWORD cWrittenChars;
    int sideNowColor = (sideNow == SIDE_WHITE ? SIDE_WHITE_COLOR : SIDE_BLACK_COLOR);
    for (short i = 0; i < FIELD_HEIGHT; i++) {
        for (short j = 0; j < FIELD_WIDTH; j++) {
            if (whoseObject(field, { j, i }) == sideNow && isAllowedBeat(field, { j, i }) == true) {
                FillConsoleOutputAttribute(hOut, (isInversionAction ? sideNowColor : HIGHLIGHT_CHECKER_COLOR),
                    1, getAbsolutePoint({ j, i }), &cWrittenChars);
            }
        }
    }
}

void showAllowedAction(HANDLE& hOut, unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj,
    bool isInversionAction)
{
    if (isAllowedBeat(field, obj))
        showAllowedBeat(hOut, field, obj, isInversionAction);
    else
        showAllowedMove(hOut, field, obj, isInversionAction);
}

void showAllowedBeat(HANDLE& hOut, unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj,
    bool isInversionAction)
{
    int sideNow = whoseObject(field, obj);
    COORD grad[4] = { {-1, -1} , { 1, -1 }, { -1, 1 }, { 1, 1 } };
    DWORD cWrittenChars;
    COORD enemy;
    COORD emptyCell;

    if (field[obj.Y][obj.X] == OBJ_WHITE || field[obj.Y][obj.X] == OBJ_BLACK) {
        for (int i = 0; i < 4; i++) {
            enemy = { obj.X + (grad + i)->X, obj.Y + (grad + i)->Y };
            emptyCell = { obj.X + ((grad + i)->X) * 2, obj.Y + ((grad + i)->Y) * 2 };
            if (whoseObject(field, enemy) == getInversionSide(sideNow)
                && field[emptyCell.Y][emptyCell.X] == OBJ_EMPTY
                && emptyCell.X >= 0 && emptyCell.X < FIELD_WIDTH
                && emptyCell.Y >= 0 && emptyCell.Y < FIELD_HEIGHT) {
                FillConsoleOutputAttribute(hOut, (isInversionAction ? CELL_COLOR : HIGHLIGHT_CHECKER_COLOR),
                    1, getAbsolutePoint(emptyCell), &cWrittenChars);
            }
        }
    }
    else {
        for (short i = 0; i < 4; i++) {
            bool enemyDetectFlag = false;
            for (short j = 1;  obj.X + grad[i].X * j >= 0 && obj.X + grad[i].X * j < FIELD_WIDTH
                && obj.Y + grad[i].Y * j >= 0 && obj.Y + grad[i].Y * j < FIELD_HEIGHT; j++) {
                COORD point = { obj.X + grad[i].X * j, obj.Y + grad[i].Y * j };
                if ((whoseObject(field, { point.X, point.Y }) == getInversionSide(sideNow) && enemyDetectFlag)
                    || whoseObject(field, { point.X, point.Y }) == sideNow) {
                    break;
                }
                else if (enemyDetectFlag == true && field[point.Y][point.X] == OBJ_EMPTY) {
                    FillConsoleOutputAttribute(hOut, (isInversionAction ? CELL_COLOR : HIGHLIGHT_CHECKER_COLOR),
                        1, getAbsolutePoint(point), &cWrittenChars);
                }
                else if (whoseObject(field, { point.X, point.Y }) == getInversionSide(sideNow)) {
                    enemyDetectFlag = true;
                }
            }
        }
    }
}

void showAllowedMove(HANDLE& hOut, unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj,
    bool isInversionAction)
{
    DWORD cWrittenChars;
    COORD grad[4] = { {-1, -1} , { 1, -1 }, { -1, 1 }, { 1, 1 } };
    COORD point;

    if (field[obj.Y][obj.X] == OBJ_WHITE || field[obj.Y][obj.X] == OBJ_BLACK) {
        for (int i = 0; i < 2; i++) {
            point = { obj.X + (grad + i)->X, obj.Y + (grad + i)->Y };
            if (field[point.Y][point.X] == OBJ_EMPTY
                && point.X >= 0 && point.X < FIELD_WIDTH && point.Y >= 0 && point.Y < FIELD_HEIGHT) {
                FillConsoleOutputAttribute(hOut, (isInversionAction ? CELL_COLOR : HIGHLIGHT_CHECKER_COLOR),
                    1, getAbsolutePoint(point), &cWrittenChars);
            }
        }
    }
    else {
        for (short i = 0; i < 4; i++) {
            for (short j = 1; (obj.X + grad[i].X * j >= 0 && obj.X + grad[i].X * j < FIELD_WIDTH
                && obj.Y + grad[i].Y * j >= 0 && obj.Y + grad[i].Y * j < FIELD_HEIGHT); j++) {
                point = { obj.X + grad[i].X * j, obj.Y + grad[i].Y * j };
                if (field[point.Y][point.X] != OBJ_EMPTY)
                    break;
                else
                    FillConsoleOutputAttribute(hOut, (isInversionAction ? CELL_COLOR : HIGHLIGHT_CHECKER_COLOR),
                        1, getAbsolutePoint(point), &cWrittenChars);
            }
        }
    }
}

void rotatePlayer(int& sideNow, int& sideNowColor)
{
    sideNow = getInversionSide(sideNow);
    sideNowColor = (sideNow == SIDE_WHITE ? SIDE_WHITE_COLOR : SIDE_BLACK_COLOR);
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
                if (isAllowedMove(field, { j, i }) || isAllowedBeat(field, { j, i }))
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
    short _const = myAbs(target.X - obj.X);
    COORD grad{ (target.X - obj.X) / _const, (target.Y - obj.Y) / _const }; // градиент, напрвление движения
    return grad;
}

inline COORD getRelativePoint(COORD c)
{
    return { c.X - FIELD_POS_X, c.Y - FIELD_POS_Y };
}

inline COORD getAbsolutePoint(COORD c)
{
    return { c.X + FIELD_POS_X, c.Y + FIELD_POS_Y };
}

void takingChecker(unsigned char field[FIELD_HEIGHT][FIELD_WIDTH], COORD obj, COORD target)
{
    int sideNow = whoseObject(field, obj);
    short _const = myAbs(target.X - obj.X);
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

    INPUT_RECORD allEvents;
    DWORD count;
    COORD mousePoint;
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
        ReadConsoleInput(hIn, &allEvents, 1, &count);
        mousePoint.X = allEvents.Event.MouseEvent.dwMousePosition.X;
        mousePoint.Y = allEvents.Event.MouseEvent.dwMousePosition.Y;

        if (allEvents.EventType == MOUSE_EVENT) {
            // new game
            if (mousePoint.Y == buttNewgame.Y && mousePoint.X >= buttNewgame.X && mousePoint.X < buttNewgame.X + 8) {
                FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 8, buttNewgame, &cWrittenChars);
                if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                    *result = 0;
                    return;
                }
            } 
            else {
                FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 8, buttNewgame, &cWrittenChars);
            }
                
            // menu
            if (mousePoint.Y == buttMenu.Y && mousePoint.X >= buttMenu.X && mousePoint.X < buttMenu.X + 4) {
                FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 4, buttMenu, &cWrittenChars);
                if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                    *result = 1;
                    return;
                }
            }       
            else {
                FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 4, buttMenu, &cWrittenChars);
            }
                
            // exit
            if (mousePoint.Y == buttExit.Y && mousePoint.X >= buttExit.X && mousePoint.X < buttExit.X + 4) {
                FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 4, buttExit, &cWrittenChars);
                if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                    exit(0);
                }
            } 
            else {
                FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 4, buttExit, &cWrittenChars);
            }
        }
    }
}