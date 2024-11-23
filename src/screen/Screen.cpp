#include "Screen.h"

Screen::Screen(int width, int height, const char *title) {
    game = new Game();
    this->width = width;
    this->height = height;
    this->title = title;
    screenType = START;
    initWindow();
}

void Screen::initWindow() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(width, height, title);
    SetTargetFPS(60);
}

Screen::~Screen() = default;

void Screen::showStartScreen() {

}

void Screen::showGameOverScreen() {

}

void Screen::showVictoryScreen() {

}

void Screen::start() {
    while (!WindowShouldClose()) {
        switch (screenType) {
            case START:
                showStartScreen();
                break;

            case PLAY:
                game->startGame();

                break;

            case GAME_OVER:
                showGameOverScreen();
                break;

            case VICTORY:
                showVictoryScreen();
                break;
        }
    }
}




