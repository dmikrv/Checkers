#include "Menu.h"
#include "Sounds.h"

const int LOGO_COLOR = Colors::COLOR_CYAN;
const int SELECTED_BUTTON_COLOR = Colors::COLOR_BLUE;
const int NONSELECTED_BUTTON_COLOR = Colors::COLOR_GRAY;

enum MenuMode {
    MENU_MAIN,
};

void drawLogo(HANDLE hOut, COORD logo)
{
    SetConsoleCursorPosition(hOut, logo);
    SetConsoleTextAttribute(hOut, LOGO_COLOR);
    std::cout << "UKRAINIAN CHECKERS";
}

void drawMenu(HANDLE hOut, COORD* button, int mode)
{
    if (mode == MENU_MAIN) {
        SetConsoleCursorPosition(hOut, { button->X,button->Y });
        SetConsoleTextAttribute(hOut, NONSELECTED_BUTTON_COLOR);
        std::cout << "NEW GAME";
        SetConsoleCursorPosition(hOut, { (button + 1)->X,(button + 1)->Y });
        SetConsoleTextAttribute(hOut, NONSELECTED_BUTTON_COLOR);
        std::cout << "HOMEPAGE";
        SetConsoleCursorPosition(hOut, *(button + 2));
        SetConsoleTextAttribute(hOut, NONSELECTED_BUTTON_COLOR);
        std::cout << "EXIT";
    }
}

void menu(HANDLE hOut, HANDLE hIn, int* gameMode)
{
    COORD buttNewgame{ WINDOW_COLS / 2 - 4, 6 };
    COORD buttHomepage{ WINDOW_COLS / 2 - 4, 7 };
    COORD buttExit{ WINDOW_COLS / 2 - 2, 8 };
    COORD buttSinglePlay { WINDOW_COLS / 2 - 7, 6 };
    COORD buttTwoPlay{ WINDOW_COLS / 2 - 6, 6 };
    COORD buttBack{ WINDOW_COLS / 2 - 2, 7 };
    COORD logo{ WINDOW_COLS / 2 - 8, 2 };

    COORD buttons[]{ buttNewgame, buttHomepage, buttExit, buttSinglePlay, buttTwoPlay, buttBack };

    int mode = MENU_MAIN;
    cls(hIn);
    drawLogo(hOut, logo);
    drawMenu(hOut, buttons, mode);
    
    INPUT_RECORD allEvents;
    DWORD count;
    COORD mousePoint;
    DWORD cWrittenChars;
    bool repeatFlag[6]{true}; // saves the state of the button; whether the cursor is on it

    while (true) {
        ReadConsoleInput(hIn, &allEvents, 1, &count);
        mousePoint.X = allEvents.Event.MouseEvent.dwMousePosition.X;
        mousePoint.Y = allEvents.Event.MouseEvent.dwMousePosition.Y;

        if (allEvents.EventType == MOUSE_EVENT) {
            if (mode == MENU_MAIN) {
                // new game
                if (mousePoint.Y == buttNewgame.Y && mousePoint.X >= buttNewgame.X 
                    && mousePoint.X < buttNewgame.X + 8) {
                    if (repeatFlag[0])
                        sSelectButtonThread();
                    repeatFlag[0] = false;
                    FillConsoleOutputAttribute(hOut, (SELECTED_BUTTON_COLOR << 4), 8, buttNewgame,
                        &cWrittenChars);
                    if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED
                        && allEvents.Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
                        sPressButtonThread();
                        *gameMode = GM_TWOPLAYER;
                        return;
                    }
                }
                else {
                    FillConsoleOutputAttribute(hOut, NONSELECTED_BUTTON_COLOR, 8, buttNewgame,
                        &cWrittenChars);
                    repeatFlag[0] = true;
                }

                // homepage
                if (mousePoint.Y == buttHomepage.Y && mousePoint.X >= buttHomepage.X 
                    && mousePoint.X < buttHomepage.X + 8) {
                    if (repeatFlag[1])
                        sSelectButtonThread();
                    repeatFlag[1] = false;
                    FillConsoleOutputAttribute(hOut, (SELECTED_BUTTON_COLOR << 4), 8, buttHomepage,
                        &cWrittenChars);
                    if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED
                        && allEvents.Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
                        sPressButtonThread();
                        openUrl(GAME_WEBSITE);
                    }
                }
                else {
                    FillConsoleOutputAttribute(hOut, NONSELECTED_BUTTON_COLOR, 8, buttHomepage,
                        &cWrittenChars);
                    repeatFlag[1] = true;
                }

                // exit
                if (mousePoint.Y == buttExit.Y && mousePoint.X >= buttExit.X
                    && mousePoint.X < buttExit.X + 4) {
                    if (repeatFlag[2])
                        sSelectButtonThread();
                    repeatFlag[2] = false;
                    FillConsoleOutputAttribute(hOut, (SELECTED_BUTTON_COLOR << 4), 4, buttExit,
                        &cWrittenChars);
                    if (allEvents.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED
                        && allEvents.Event.MouseEvent.dwEventFlags != MOUSE_MOVED) {
                        sPressButtonThread();
                        exit(0);
                    }
                }
                else {
                    FillConsoleOutputAttribute(hOut, NONSELECTED_BUTTON_COLOR, 4, buttExit,
                        &cWrittenChars);
                    repeatFlag[2] = true;
                }
            }
        }
    }
}