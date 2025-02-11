#include "../catch.hpp"
#include "../../main/controller/Game.h"

TEST_CASE("Verify game logic") {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(660, 690, "Test game");
    SetTargetFPS(60);
    Game* game = new Game(660, 690);

    CHECK(game->getSnake()->getBody().size() == 1);
    CHECK(game->getSnake()->getLength() == 1);
    CHECK(game->getSnake()->getDirection() == RIGHT);
    CHECK(game->getGameStatus() == RUNNING);
    CHECK(game->getScore() == 0);
    CHECK(game->getFood().size() == 1);
    CHECK(game->isPaused() == false);

    SECTION("Snake eats and grows") {
        game->addFood(CELL_SIZE * 6, CELL_SIZE * 5);
        REQUIRE(game->getFood().size() == 2);

        game->setSnakePosition();
        REQUIRE(game->getSnake()->getHead()->getX() == CELL_SIZE * 6);
        REQUIRE(game->getSnake()->getHead()->getY() == CELL_SIZE * 5);
        REQUIRE(game->getSnake()->getLength() == 2);
        REQUIRE(game->getFood().size() == 1);
        REQUIRE(game->getScore() == 1);
    }

    SECTION("Lose game" "[Hit itself]") {
        for (int i = 0; i < 4; ++i) {
            game->addFood(CELL_SIZE * (i + 6), CELL_SIZE * 5);
            game->setSnakePosition();
        }
        REQUIRE(game->getSnake()->getLength() == 5);

        game->getSnake()->setDirection(DOWN);
        game->setSnakePosition();
        game->getSnake()->setDirection(LEFT);
        game->setSnakePosition();
        game->getSnake()->setDirection(UP);
        game->setSnakePosition();

        REQUIRE(game->getGameStatus() == LOSS);
    }

    SECTION("Lose game" "[Hit boundary]") {
        game->getSnake()->setDirection(UP);
        for (int i = 0; i < 4; ++i) {
            game->setSnakePosition();
        }
        REQUIRE(game->getGameStatus() == LOSS);
    }

    delete game;
    CloseWindow();
}