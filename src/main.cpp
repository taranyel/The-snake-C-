#include "controller/Game.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define START_SPEED 150
#define SNAKE_WIDTH 30


int main() {
    Game* game = new Game(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake", START_SPEED, SNAKE_WIDTH);
    game->startGame();
    delete game;
    return 0;
}

