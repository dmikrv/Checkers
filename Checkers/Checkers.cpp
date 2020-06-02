#include "Checkers.h"
#include "Menu.h"
#include "Game.h"
#include "EnterName.h"

int main()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cursorVisible(hOut, false);
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hIn, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    SetConsoleTitleA(WINDOW_TITLE);
    resizeWindow(WINDOW_COLS, WINDOW_LINES);
    changeFont(hOut, WINDOW_FONT_SIZE, WINDOW_FONT_NAME);

    char playerName1[BUFFER_SIZE] = "PLAYER1";
    char playerName2[BUFFER_SIZE] = "PLAYER2";

    int gameMode;
    int buff = 1; // result of the return of the func
    while (true) {
        // buff = 0 : game
        // buff = 1 : menu
        while (buff == 1) {
            menu(hOut, hIn, &gameMode);
            enterPlayersName(hOut, hIn, playerName1, playerName2, BUFFER_SIZE, &buff);
        }
        game(hOut, hIn, playerName1, playerName2, gameMode, &buff);
    }
}

