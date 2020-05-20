#pragma once

#define WINDOW_COLS 30
#define WINDOW_LINES 15
#define WINDOW_TITLE "Russian Checkers"
#define WINDOW_FONT_SIZE 36
#define WINDOW_FONT_NAME L"Consolas"

#define FIELD_WIDTH 8
#define FIELD_HEIGHT 8

#define BUTTONS_POS_X 19
#define BUTTONS_POS_Y 1
#define SIDESHOW_POS_X 18
#define SIDESHOW_POS_Y 5
#define SHOW_POS_X 18
#define SHOW_POS_Y 3
#define NUMBERING_POS_X 5
#define NUMBERING_POS_Y 0
#define FIELD_POS_X 6
#define FIELD_POS_Y 1
#define LEGENDMAP_POS_X 2
#define LEGENDMAP_POS_Y 11

enum Side {
    SIDE_WHITE,
    SIDE_BLACK,
};

enum GameMode {
    GM_WHITE,
    GM_BLACK,
    GM_WATCHER,
    GM_TWOPLAYER,
};