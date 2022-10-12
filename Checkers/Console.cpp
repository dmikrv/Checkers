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

void drawPosition(HANDLE hout, int color, COORD where, COORD what)
{
    SetConsoleCursorPosition(hout, where);
    SetConsoleTextAttribute(hout, color);
    std::cout << "Position: ";
    where.Y++;
    SetConsoleCursorPosition(hout, where);
    std::cout << "  X: " << what.X << "   ";
    where.Y++;
    SetConsoleCursorPosition(hout, where);
    std::cout << "  Y: " << what.Y << "   ";
}

void changeFont(HANDLE hOut, int fontSize, const wchar_t* fontName)
{
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.dwFontSize.Y = fontSize;    // Height
    wcscpy_s(cfi.FaceName, fontName); // Choose your font
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

void openUrl(LPCSTR url)
{
    ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWDEFAULT);
}

void cls(HANDLE hIn)
{
    system("CLS");
    SetConsoleMode(hIn, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
}

void drawText(HANDLE hOut, short posY, int windowCols, char* text, int color)
{
    SetConsoleTextAttribute(hOut, color);
    SetConsoleCursorPosition(hOut, { static_cast<short>((windowCols - getLettersInWord(text)) / 2),
        posY });
    std::cout << text;
}

int getLettersInWord(char* text)
{
    int i = 0;
    char* temp = text;
    for (; *temp; temp++) {
        i++;
    }
    return i;
}

bool strcatK(char* destination, int buffSize, const char* source)
{
    char* ptr = destination;
    int i = 0;
    for (; i < buffSize; i++) {
        if (*ptr == '\0')
            break;
        ptr++;
    }
    int lettersInSource = getLettersInWord(const_cast<char*>(source));
    if (lettersInSource > buffSize - i)
        return 0;
    char* sourcePtr = const_cast<char*>(source);
    for (int j = 0; j < lettersInSource; j++) {
        *ptr = *sourcePtr;
        ptr++;
        sourcePtr++;
    }
    return 1;
}

bool strcatK(char* destination, int buffSize, const char source)
{
    char* ptr = destination;
    int i = 0;
    for (; i < buffSize; i++) {
        if (*ptr == '\0')
            break;
        ptr++;
    }
    if (i != buffSize - 1) {
        *ptr = source;
        return 1;
    }
    return 0;
}

bool strcatK(char* destination, int buffSize, const int source)
{
    char* ptr = destination;
    int i = 0;
    for (; i < buffSize; i++) {
        if (*ptr == '\0')
            break;
        ptr++;
    }
    if (i != buffSize - 1) {
        *ptr = source + 48;
        return 1;
    }
    return 0;
}

void backspaceText(char* text)
{
    char* ptr = text;
    while (*ptr != '\0')
        ptr++;
    ptr--;
    *ptr = '\0';
}