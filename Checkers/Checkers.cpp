#include <Windows.h>
#include <iostream>
#include "Game.h"

int main()
{
    SetConsoleTitleA(WINDOW_TITLE);
    resizeWindow(WINDOW_COLS, WINDOW_LINES);

    register HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cursorVisible(hOut, false);
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hIn, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    //cfi.nFont = 0;
    cfi.dwFontSize.Y = WINDOW_FONT_SIZE;    // Height
    //cfi.FontFamily = FF_DONTCARE;
    //cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, WINDOW_FONT_NAME); // Choose your font
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);

    unsigned char field[FIELD_HEIGHT][FIELD_WIDTH];
    int gameMode = GM_TWOPLAYER;
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

    //changeFont(hOut, { WINDOW_FONT_SIZE_WIDTH , WINDOW_FONT_SIZE_HEIGHT });
    drawNumbering(hOut, NUMBERING_POS_X, NUMBERING_POS_Y);
    drawLegendMap(hOut, LEGENDMAP_POS_X, LEGENDMAP_POS_Y);
    drawField(hOut, field, FIELD_POS_X, FIELD_POS_Y);
    drawCount(hOut, countBlack, countWhite);
    drawPosition(hOut, SHOW_POS_X, SHOW_POS_Y, { 0, 0 });
    drawSideshow(hOut, sideNow, SIDESHOW_POS_X, SIDESHOW_POS_Y);

    while (true) {
        ReadConsoleInput(hIn, &all_events, 1, &count);
        point.X = all_events.Event.MouseEvent.dwMousePosition.X;
        point.Y = all_events.Event.MouseEvent.dwMousePosition.Y;

        if (all_events.EventType == MOUSE_EVENT) {
            if (all_events.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                // проверка на отсутствие ходов
                if (isHaveAct(field, sideNow) == false) {
                    endGame(hOut, getInversionSide(sideNow));
                }

                // получение координат в рамках игрового поля
                COORD pointC = { point.X - FIELD_POS_X, point.Y - FIELD_POS_Y }; // point cut
                COORD selectedCoordC = { selectedCoord.X - FIELD_POS_X, selectedCoord.Y - FIELD_POS_Y }; // selected cut
                
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
                // проверка на сделанный ход
                if (doneFlag && !beatFlag) {
                    doneFlag = false;
                    drawField(hOut, field, FIELD_POS_X, FIELD_POS_Y);
                    drawCount(hOut, countBlack, countWhite);

                    // проверка на наличие шашек
                    if (sideNow == SIDE_WHITE && countBlack == 0)
                        endGame(hOut, sideNow);
                    else if (sideNow == SIDE_BLACK && countWhite == 0)
                        endGame(hOut, sideNow);

                    if (gameMode == GM_TWOPLAYER) {
                        rotatePlayer(sideNow, sideNowCol);
                        rotateField(field);
                        Sleep(450);
                        drawField(hOut, field, FIELD_POS_X, FIELD_POS_Y);
                    }

                    drawSideshow(hOut, sideNow, SIDESHOW_POS_X, SIDESHOW_POS_Y);
                }
            }
            drawPosition(hOut, SHOW_POS_X, SHOW_POS_Y, point);
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
