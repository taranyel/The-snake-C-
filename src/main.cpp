#include "controller/Game.h"

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700
#define START_SPEED 3
#define SNAKE_WIDTH 30

int main() {
    Game* game = new Game(SCREEN_WIDTH, SCREEN_HEIGHT, "First window", START_SPEED, SNAKE_WIDTH);
    game->startGame();
    delete game;
    return 0;
}
