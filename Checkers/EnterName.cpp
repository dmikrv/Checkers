#include "EnterName.h"

#define KEYBOARD_NUMBER_POS_Y 6
#define KEYBOARD_NUMBER_POS_X 4
#define KEYBOARD_BACKSPACE_POS_Y 6
#define KEYBOARD_BACKSPACE_POS_X 26
#define KEYBOARD_LATIN1_POS_Y 8
#define KEYBOARD_LATIN1_POS_X 2
#define KEYBOARD_LATIN2_POS_Y 10
#define KEYBOARD_LATIN2_POS_X 2

#define WORD_POS_Y 3

void drawKeyboard(HANDLE& hOut)
{
    SetConsoleTextAttribute(hOut, Colors::COLOR_WHITE);
    SetConsoleCursorPosition(hOut, { KEYBOARD_NUMBER_POS_X, KEYBOARD_NUMBER_POS_Y });
    for (int i = 0; i <= 9; i++) {
        std::cout << i << " ";
    }

    SetConsoleTextAttribute(hOut, Colors::COLOR_WHITE);
    SetConsoleCursorPosition(hOut, { KEYBOARD_BACKSPACE_POS_X, KEYBOARD_BACKSPACE_POS_Y });
    std::cout << (char)196;

    SetConsoleTextAttribute(hOut, Colors::COLOR_WHITE);
    SetConsoleCursorPosition(hOut, { KEYBOARD_LATIN1_POS_X, KEYBOARD_LATIN1_POS_Y });
    for (int i = 65; i <= 90; i++) {
        std::cout << (char)i << " ";
        if (i == 65 + 12)
            SetConsoleCursorPosition(hOut, { KEYBOARD_LATIN2_POS_X, KEYBOARD_LATIN2_POS_Y });
    }
}

void drawButtons(HANDLE& hOut, COORD back, COORD next)
{
    SetConsoleTextAttribute(hOut, Colors::COLOR_GRAY);
    SetConsoleCursorPosition(hOut, back);
    std::cout << "BACK";
    SetConsoleCursorPosition(hOut, next);
    std::cout << "NEXT";
}

void drawName(HANDLE& hOut, char* text, short posY)
{
    SetConsoleTextAttribute(hOut, Colors::COLOR_RED);
    SetConsoleCursorPosition(hOut, { static_cast<short>((WINDOW_COLS - getLettersInWord(text)) / 2),
        posY });
    std::cout << text;
}

void enterPlayerName(HANDLE& hOut, HANDLE& hIn, char* playerName, int buffSize, int* result)
{
    drawKeyboard(hOut);
    COORD buttonBack = { 3, WINDOW_LINES - 2 };
    COORD buttonNext = { WINDOW_COLS - 7, WINDOW_LINES - 2 };
    drawButtons(hOut, buttonBack, buttonNext);

    INPUT_RECORD allEvents;
    DWORD count;
    COORD mousePoint;
    DWORD cWrittenChars;

    SetConsoleMode(hIn, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    while (true) {
        ReadConsoleInput(hIn, &allEvents, 1, &count);
        mousePoint.X = allEvents.Event.MouseEvent.dwMousePosition.X;
        mousePoint.Y = allEvents.Event.MouseEvent.dwMousePosition.Y;

        drawName(hOut, playerName, WORD_POS_Y);
        FillConsoleOutputAttribute(hOut, Colors::COLOR_WHITE, WINDOW_COLS, { 0, KEYBOARD_NUMBER_POS_Y },
            &cWrittenChars);
        FillConsoleOutputAttribute(hOut, Colors::COLOR_WHITE, WINDOW_COLS, { 0, KEYBOARD_LATIN1_POS_Y },
            &cWrittenChars);
        FillConsoleOutputAttribute(hOut, Colors::COLOR_WHITE, WINDOW_COLS, { 0, KEYBOARD_LATIN2_POS_Y },
            &cWrittenChars);
        FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, WINDOW_COLS, { 0, buttonNext.Y },
            &cWrittenChars);

        // numbering
        if (mousePoint.X >= KEYBOARD_NUMBER_POS_X && mousePoint.X < KEYBOARD_NUMBER_POS_X + 19
            && mousePoint.Y == KEYBOARD_NUMBER_POS_Y && (mousePoint.X % 2 == 0) 
            && strlen(playerName) < buffSize - 1 && playerName[0] != '\0') {
            FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 1, mousePoint, &cWrittenChars);
            if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED
                && allEvents.Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
                sSelectCheckerThread();
                int num = (mousePoint.X - KEYBOARD_NUMBER_POS_X) / 2;
                strcatK(playerName, buffSize, num);
            }
        }
        // backspace
        else if (mousePoint.X == KEYBOARD_BACKSPACE_POS_X && mousePoint.Y == KEYBOARD_BACKSPACE_POS_Y
            && playerName[0] != '\0') {
            FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 1, mousePoint, &cWrittenChars);
            if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED
                && allEvents.Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
                sSelectCheckerThread();
                SetConsoleCursorPosition(hOut, { 0, WORD_POS_Y });
                for (int i = 0; i < 30; i++)
                    std::cout << " ";
                backspaceText(playerName);
            }
        }
        // keyboard first part
        else if (mousePoint.Y == KEYBOARD_LATIN1_POS_Y && (mousePoint.X % 2 == 0) 
            && strlen(playerName) < buffSize - 1
            && mousePoint.X >= KEYBOARD_LATIN1_POS_X && mousePoint.X <= KEYBOARD_LATIN1_POS_X + 24) {
            FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 1, mousePoint, &cWrittenChars);
            if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED
                && allEvents.Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
                sSelectCheckerThread();
                char letter = (mousePoint.X - KEYBOARD_LATIN1_POS_X) / 2 + 65;
                strcatK(playerName, buffSize, letter);
            }
        }
        // keyboard second part
        else if (mousePoint.Y == KEYBOARD_LATIN2_POS_Y && (mousePoint.X % 2 == 0) 
            && strlen(playerName) < buffSize - 1
            && mousePoint.X >= KEYBOARD_LATIN2_POS_X && mousePoint.X <= KEYBOARD_LATIN2_POS_X + 24) {
            FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 1, mousePoint, &cWrittenChars);
            if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED
                && allEvents.Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
                sSelectCheckerThread();
                char letter = (mousePoint.X - KEYBOARD_LATIN2_POS_X) / 2 + 78;
                strcatK(playerName, buffSize, letter);
            }
        }
        // button Cancel
        else if (mousePoint.Y == buttonBack.Y && mousePoint.X >= buttonBack.X 
            && mousePoint.X < buttonBack.X + 4) {
            FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 4, buttonBack, &cWrittenChars);
            if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED
                && allEvents.Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
                sPressButtonThread();
                *result = 0;
                return;
            }
        }
        // button Next
        if (mousePoint.Y == buttonNext.Y && mousePoint.X >= buttonNext.X 
            && mousePoint.X < buttonNext.X + 4
            && strlen(playerName) > 0) {
            FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 4, buttonNext, &cWrittenChars);
            if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED
                && allEvents.Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
                sPressButtonThread();
                *result = 1;
                return;
            }
        }
        FlushConsoleInputBuffer(hIn);
    }
}

void enterPlayersName(HANDLE& hOut, HANDLE& hIn, char* playerName1, char* playerName2, 
    int buffSize, int* result)
{
    int buff = 0;
    // buff = 0 : button Cancel
    // buff = 1 : button Next
    while (true) {
        cls(hIn);
        drawText(hOut, 1, WINDOW_COLS, const_cast<char*>("Enter first player name"), Colors::COLOR_WHITE);
        enterPlayerName(hOut, hIn, playerName1, buffSize, &buff);
        if (buff == 0) {
            *result = 1; // menu
            return;
        }
        cls(hIn);
        drawText(hOut, 1, WINDOW_COLS, const_cast<char*>("Enter second player name"),
            Colors::COLOR_WHITE);
        enterPlayerName(hOut, hIn, playerName2, buffSize, &buff);
        if (buff == 1 && strcmp(playerName1, playerName2) != 0) {
            *result = 0; // game
            return;
        }
    }
}

