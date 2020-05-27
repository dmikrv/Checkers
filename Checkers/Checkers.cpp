#include "Checkers.h"
#include "Menu.h"
#include "Game.h"

int main()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cursorVisible(hOut, false);
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hIn, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    SetConsoleTitleA(WINDOW_TITLE);
    resizeWindow(WINDOW_COLS, WINDOW_LINES);
    changeFont(hOut, WINDOW_FONT_SIZE, WINDOW_FONT_NAME);

    int gameMode;
    int buff = 1; // result of the return of the func. game
    while (true) {
        if (buff == 1)
            menu(hOut, hIn, &gameMode);
        game(hOut, hIn, gameMode, &buff);
    }
}
