#include "Console.h"

void cursorVisible(HANDLE hOut, bool isVisible)
{
    CONSOLE_CURSOR_INFO cur;
    cur.bVisible = isVisible;
    cur.dwSize = 100;
    SetConsoleCursorInfo(hOut, &cur);
}

void dragWindow(int X, int Y)
{
    int res_width = GetSystemMetrics(SM_CXSCREEN);
    int res_height = GetSystemMetrics(SM_CYSCREEN);
    MoveWindow(GetConsoleWindow(), X, Y, res_width, res_height, true);
}

void resizeWindow(int cols, int lines)
{
    char str[30];
    wsprintfA(str, "mode con cols=%d lines=%d", cols, lines);
    system(str);
}

void drawPosition(HANDLE hout, COORD where, COORD what)
{
    SetConsoleCursorPosition(hout, where);
    SetConsoleTextAttribute(hout, Colors::COLOR_RED);
    std::cout << "Position: ";
    where.Y++;
    SetConsoleCursorPosition(hout, where);
    std::cout << "  X: " << what.X << "   ";
    where.Y++;
    SetConsoleCursorPosition(hout, where);
    std::cout << "  Y: " << what.Y << "   ";
}

void changeFont(HANDLE hOut, COORD dwFontSize)
{
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = dwFontSize.X;                   // Width of each character in the font
    cfi.dwFontSize.Y = dwFontSize.Y;                  // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    //wcscpy_s(cfi.FaceName, WINDOW_FONT_NAME); // Choose your font
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}