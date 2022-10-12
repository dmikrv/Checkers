#pragma once

#define WINDOW_COLS 30
#define WINDOW_LINES 15
#define WINDOW_TITLE "Ukrainian Checkers"
#define WINDOW_FONT_SIZE 36
#define WINDOW_FONT_NAME L"Consolas"

#define FIELD_WIDTH 8
#define FIELD_HEIGHT 8

#define GAME_WEBSITE "https://github.com/fan1ech00/checkers"
#define ROTATE_FIELD_SLEEP 500
#define BUFFER_SIZE 12

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