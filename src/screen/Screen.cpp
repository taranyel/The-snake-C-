#include "Screen.h"

Screen::Screen(int width, int height, const char *title) {
    this->width = width;
    this->height = height;
    this->title = title;
    screenType = START;
    initWindow();
    game = new Game();
}

void Screen::initWindow() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(width, height, title);
    SetTargetFPS(60);
}

Screen::~Screen() {
    delete game;
}

void Screen::showStartScreen() const {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawRectangle(0, 0, width, height, GREEN);
    DrawText("START", 200, 180, 70, DARKGREEN);
    DrawText("PRESS ENTER to START NEW GAME", 45, 290, 30, DARKGREEN);
    DrawText("Game controls:", 45, 450, 25, BLACK);
    DrawText("- arrows to MOVE", 55, 490, 25, DARKGREEN);
    DrawText("- Q to QUIT", 55, 525, 25, DARKGREEN);
    DrawText("- H to HELP", 55, 560, 25, DARKGREEN);

    EndDrawing();
}

void Screen::showGameOverScreen() const {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawRectangle(0, 0, width, height, RED);
    DrawText("GAME OVER", 115, 180, 70, BLACK);

    std::string scoreText = "SCORE: ";
    scoreText.append(std::to_string(game->getScore()));
    DrawText(scoreText.c_str(), 240, 290, 35, BLACK);

    DrawText("PRESS ENTER to START NEW GAME", 45, 360, 30, DARKGRAY);

    EndDrawing();
}

void Screen::showVictoryScreen() const {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawRectangle(0, 0, width, height, YELLOW);
    DrawText("VICTORY!!!", 170, 180, 70, BROWN);

    std::string scoreText = "SCORE: ";
    scoreText.append(std::to_string(game->getScore()));
    DrawText(scoreText.c_str(), 240, 290, 35, BLACK);

    DrawText("PRESS ENTER to START NEW GAME", 45, 360, 30, DARKGRAY);

    EndDrawing();
}

void Screen::showHelpScreen() const {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawRectangle(0, 0, width, height, LIGHTGRAY);
    DrawText("HELP", 230, 150, 70, BROWN);
    DrawText("PRESS ENTER to START NEW GAME", 45, 260, 30, BROWN);
    DrawText("Main rules:", 45, 400, 30, BLACK);
    DrawText("- eat food", 55, 460, 25, BROWN);
    DrawText("- to fill the entire playing field with a snake", 55, 505, 25, BROWN);
    DrawText("- snake speed will increase every 15 seconds ;)", 55, 545, 25, BROWN);

    EndDrawing();
}

void Screen::start() {
    while (!WindowShouldClose()) {
        width = GetScreenWidth();
        height = GetScreenHeight();

        switch (screenType) {
            case START:
                showStartScreen();
                break;

            case PLAY:
                delete game;
                game = new Game();
                game->startGame();
                setScreenTypeAfterGame();
                break;

            case GAME_OVER:
                showGameOverScreen();
                break;

            case VICTORY:
                showVictoryScreen();
                break;

            case HELP:
                showHelpScreen();
                break;
        }

        if (screenType != PLAY) {
            if (!startPlay()) {
                break;
            }
        }
    }

    CloseWindow();
}

void Screen::setScreenTypeAfterGame() {
    switch (game->getGameStatus()) {
        case LOSS:
            screenType = GAME_OVER;
            break;

        case WON:
            screenType = VICTORY;
            break;

        default:
            screenType = START;
    }
}

bool Screen::startPlay() {
    if (IsKeyPressed(KEY_ENTER)) {
        screenType = PLAY;
    } else if (IsKeyPressed(KEY_H)) {
        screenType = HELP;
    } else if (IsKeyPressed(KEY_Q)) {
        return false;
    }
    return true;
}

