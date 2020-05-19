#include <Windows.h>
#include <iostream>
#include "Menu.h"
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
   
    int gameMode;
    int buff = 1;
    while (true) {
        if (buff == 1)
            menu(hOut, hIn, &gameMode);
        game(hOut, hIn, gameMode, &buff);
    }
}
