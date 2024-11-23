#include <queue>
#include <mutex>
#include <cstdlib>
#include <thread>
#include <iostream>
#include <condition_variable>
#include "../entity/Snake.h"
#include "GameStatus.h"

#define CELL_SIZE 30
#define START_SPEED 150

class Game {
private:
    Snake *snake;
    std::mutex snakeMutex, foodMutex, commandsMutex;
    std::vector<Cell *> food;
    std::queue<int> commands;
    bool paused;
    std::condition_variable cv;
    std::time_t startSnakeTimer, startFoodTimer;
    GameStatus gameStatus;

public:
    Game();

    ~Game();

    void startGame();

    void endGame(GameStatus status);

    void pauseGame();

    void draw();

    void moveSnake();

    void controlGame();

    void generateFood();

    void eatFood();

    static int generateNumber(int max);

    int checkBoundary(int a, int max);

    void waitForInput();

    void controlSpeed();

    bool isInArray(int x, int y, const std::vector<Cell *>& array, int startIndex);

    bool isGameOver();
};
