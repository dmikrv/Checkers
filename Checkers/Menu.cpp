#include "Menu.h"

enum MenuMode {
    MENU_MAIN,
    MENU_GM_SELECTION,
    MENU_SIDE_SELECTION,
};

void drawLogo(HANDLE& hOut, COORD logo)
{
    SetConsoleCursorPosition(hOut, logo);
    SetConsoleTextAttribute(hOut, Colors::COLOR_CYAN);
    std::cout << "RUSSIAN CHECKERS";
}

void drawMenu(HANDLE& hOut, HANDLE& hIn, COORD* button, int mode)
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
    COORD logo{ WINDOW_COLS / 2 - 8, 2 };

    COORD buttons[]{ buttNewgame, buttHomepage, buttExit, buttSinglePlay, buttTwoPlay, buttBack };

    int mode = MENU_MAIN;
    system("cls");
    SetConsoleMode(hIn, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    drawLogo(hOut, logo);
    drawMenu(hOut, hIn, buttons, mode);
    
    INPUT_RECORD allEvents;
    DWORD count;
    COORD mousePoint;
    DWORD cWrittenChars;

    while (true) {
        ReadConsoleInput(hIn, &allEvents, 1, &count);
        mousePoint.X = allEvents.Event.MouseEvent.dwMousePosition.X;
        mousePoint.Y = allEvents.Event.MouseEvent.dwMousePosition.Y;

        if (allEvents.EventType == MOUSE_EVENT) {
            if (mode == MENU_MAIN) {
                // new game
                if (mousePoint.Y == buttNewgame.Y && mousePoint.X >= buttNewgame.X 
                    && mousePoint.X < buttNewgame.X + 8) {
                    FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 8, buttNewgame, &cWrittenChars);
                    if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                        mode = MENU_GM_SELECTION;
                        drawMenu(hOut, hIn, buttons, mode);
                    }
                }
                else {
                    FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 8, buttNewgame, &cWrittenChars);
                }

                // homepage
                if (mousePoint.Y == buttHomepage.Y && mousePoint.X >= buttHomepage.X 
                    && mousePoint.X < buttHomepage.X + 8) {
                    FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 8, buttHomepage, &cWrittenChars);
                    if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                        system("start https://gitlab.com/fan1ech00/checkers");
                    }
                }
                else {
                    FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 8, buttHomepage, &cWrittenChars);
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
            else if (mode == MENU_GM_SELECTION) {
                // single player
                //if (mousePoint.Y == buttSinglePlay.Y && mousePoint.X >= buttSinglePlay.X && mousePoint.X < buttSinglePlay.X + 13) {
                //    FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 13, buttSinglePlay, &cWrittenChars);
                //    if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                //        ;
                //    }
                //}
                //else {
                //    FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 13, buttSinglePlay, &cWrittenChars);
                //}

                // two player
                if (mousePoint.Y == buttTwoPlay.Y && mousePoint.X >= buttTwoPlay.X 
                    && mousePoint.X < buttTwoPlay.X + 11) {
                    FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 11, buttTwoPlay, &cWrittenChars);
                    if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                        *gameMode = GM_TWOPLAYER;
                        return;
                    }
                }
                else {
                    FillConsoleOutputAttribute(hOut, Colors::COLOR_GRAY, 11, buttTwoPlay, &cWrittenChars);
                }

                // back
                if (mousePoint.Y == buttBack.Y && mousePoint.X >= buttBack.X && mousePoint.X < buttBack.X + 4) {
                    FillConsoleOutputAttribute(hOut, (Colors::COLOR_BLUE << 4), 4, buttBack, &cWrittenChars);
                    if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
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