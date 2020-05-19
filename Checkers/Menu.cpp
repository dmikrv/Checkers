#include "Menu.h"

void drawLogo(HANDLE& hOut)
{
    COORD logo{ WINDOW_COLS / 2 - 8, 2 };
    SetConsoleCursorPosition(hOut, logo);
    SetConsoleTextAttribute(hOut, Colors::COLOR_CYAN);
    std::cout << "RUSSIAN CHECKERS";
}

void drawMenu(HANDLE& hOut, HANDLE& hIn, COORD *button, int mode)
{
    if (mode == MENU_MAIN) {
        SetConsoleCursorPosition(hOut, { button->X - 3,button->Y });
        SetConsoleTextAttribute(hOut, Colors::COLOR_GRAY);
        std::cout << "   NEW GAME   ";
        SetConsoleCursorPosition(hOut, { (button+1)->X - 3,(button + 1)->Y });
        SetConsoleTextAttribute(hOut, Colors::COLOR_GRAY);
        std::cout << "   HOMEPAGE   ";
        SetConsoleCursorPosition(hOut, *(button + 2));
        SetConsoleTextAttribute(hOut, Colors::COLOR_GRAY);
        std::cout << "EXIT";
    }
    else if (mode == MENU_GM_SELECTION) {
        SetConsoleCursorPosition(hOut, *(button + 3));
        SetConsoleTextAttribute(hOut, Colors::COLOR_GRAY);
        std::cout << "SINGLE PLAYER";
        SetConsoleCursorPosition(hOut, *(button + 4));
        SetConsoleTextAttribute(hOut, Colors::COLOR_GRAY);
        std::cout << "TWO PLAYERS";
        SetConsoleCursorPosition(hOut, *(button + 5));
        SetConsoleTextAttribute(hOut, Colors::COLOR_GRAY);
        std::cout << "BACK";
    }
}

void menu(HANDLE& hOut, HANDLE& hIn, int* gameMode)
{
    COORD buttNewgame{ WINDOW_COLS / 2 - 4, 6 };
    COORD buttHomepage{ WINDOW_COLS / 2 - 4, 7 };
    COORD buttExit{ WINDOW_COLS / 2 - 2, 8 };
    COORD buttSinglePlay { WINDOW_COLS / 2 - 7, 6 };
    COORD buttTwoPlay{ WINDOW_COLS / 2 - 6, 7 };
    COORD buttBack{ WINDOW_COLS / 2 - 2, 8 };

    COORD buttons[]{ buttNewgame, buttHomepage, buttExit, buttSinglePlay, buttTwoPlay, buttBack };

    int mode = MENU_MAIN;
    system("cls");
    SetConsoleMode(hIn, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    drawLogo(hOut);
    drawMenu(hOut, hIn, buttons, mode);
    
    INPUT_RECORD all_events;
    DWORD count;
    COORD point;
    DWORD cWrittenChars;

    while (true) {
        ReadConsoleInput(hIn, &all_events, 1, &count);
        point.X = all_events.Event.MouseEvent.dwMousePosition.X;
        point.Y = all_events.Event.MouseEvent.dwMousePosition.Y;

        if (all_events.EventType == MOUSE_EVENT) {
            if (mode == MENU_MAIN) {
                // new game
                if (point.Y == buttNewgame.Y && point.X >= buttNewgame.X && point.X < buttNewgame.X + 8) {
                    FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 8, buttNewgame, &cWrittenChars);
                    if (all_events.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                        mode = MENU_GM_SELECTION;
                        drawMenu(hOut, hIn, buttons, mode);
                    }
                }
                else {
                    FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 8, buttNewgame, &cWrittenChars);
                }

                // homepage
                if (point.Y == buttHomepage.Y && point.X >= buttHomepage.X && point.X < buttHomepage.X + 8) {
                    FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 8, buttHomepage, &cWrittenChars);
                    if (all_events.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                        system("start https://gitlab.com/fan1ech00/checkers");
                    }
                }
                else {
                    FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 8, buttHomepage, &cWrittenChars);
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
            else if (mode == MENU_GM_SELECTION) {
                // single player
                //if (point.Y == buttSinglePlay.Y && point.X >= buttSinglePlay.X && point.X < buttSinglePlay.X + 13) {
                //    FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 13, buttSinglePlay, &cWrittenChars);
                //    if (all_events.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                //        ;
                //    }
                //}
                //else {
                //    FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 13, buttSinglePlay, &cWrittenChars);
                //}

                // two player
                if (point.Y == buttTwoPlay.Y && point.X >= buttTwoPlay.X && point.X < buttTwoPlay.X + 11) {
                    FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 11, buttTwoPlay, &cWrittenChars);
                    if (all_events.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                        *gameMode = GM_TWOPLAYER;
                        return;
                    }
                }
                else {
                    FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 11, buttTwoPlay, &cWrittenChars);
                }

                // back
                if (point.Y == buttBack.Y && point.X >= buttBack.X && point.X < buttBack.X + 4) {
                    FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 4, buttBack, &cWrittenChars);
                    if (all_events.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                        mode = MENU_MAIN;
                        drawMenu(hOut, hIn, buttons, mode);
                    }
                }
                else {
                    FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 4, buttBack, &cWrittenChars);
                }
            }
        }
    }
}