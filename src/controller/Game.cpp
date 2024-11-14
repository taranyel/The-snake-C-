#include "Game.h"

Game::Game(int screenWidth, int screenHeight, const char *title, int startSpeed, int snakeWidth) {
    window = new Window(screenWidth, screenHeight, title);
    window->initWindow();
    snake = new Snake(screenWidth / 2, screenHeight / 2, startSpeed, startSpeed, snakeWidth);
}

Game::~Game() {
    delete window;
    delete snake;
}

void Game::startGame() {
    bool pause = false;

    while (!WindowShouldClose()) {
        window->setWidth(GetScreenWidth());
        window->setHeight(GetScreenHeight());

        if (IsKeyPressed(KEY_SPACE)) {
            pause = !pause;
        }

        if (!pause) {
            handleGame();
        }

        draw();
    }
    CloseWindow();
}

void Game::handleGame() {
    if (IsKeyDown(KEY_RIGHT)) {
        snake->setDirection(RIGHT);

    } else if (IsKeyDown(KEY_LEFT)) {
        snake->setDirection(LEFT);

    } else if (IsKeyDown(KEY_UP)) {
        snake->setDirection(UP);

    } else if (IsKeyDown(KEY_DOWN)) {
        snake->setDirection(DOWN);
    }
    turnSnake();
}

void Game::endGame() {

}

void Game::draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (Coordinate* coordinate : snake->getBody()) {
        DrawRectangle(coordinate->getX(), coordinate->getY(), snake->getWidth(), snake->getWidth(), MAROON);
    }

    EndDrawing();
}

void Game::turnSnake() {
    switch (snake->getDirection()) {
        case UP:
            moveSnake(0, -snake->getSpeed().getY());
            break;
        case DOWN:
            moveSnake(0, snake->getSpeed().getY());
            break;
        case RIGHT:
            moveSnake(snake->getSpeed().getX(), 0);
            break;
        case LEFT:
            moveSnake(-snake->getSpeed().getX(), 0);
            break;
    }
}

void Game::moveSnake(int x, int y) {
    for (Coordinate *coordinate: snake->getBody()) {
        if (coordinate->getX() > window->getWidth()) {
            coordinate->setX(0);

        } else if (coordinate->getX() < 0) {
            coordinate->setX(window->getWidth());

        } else {
            coordinate->setX(coordinate->getX() + x);
        }

        if (coordinate->getY() > window->getHeight()) {
            coordinate->setY(0);

        } else if (coordinate->getY() < 0) {
            coordinate->setY(window->getHeight());

        } else {
            coordinate->setY(coordinate->getY() + y);
        }
    }
}